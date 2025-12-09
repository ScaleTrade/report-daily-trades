#include "PluginInterface.hpp"

#include <iomanip>

extern "C" void AboutReport(rapidjson::Value& request,
                            rapidjson::Value& response,
                            rapidjson::Document::AllocatorType& allocator,
                            CServerInterface* server) {
    response.AddMember("version", 1, allocator);
    response.AddMember("name", Value().SetString("Daily Trades report", allocator), allocator);
    response.AddMember("description",
        Value().SetString("Trading operations of selected trader groups for the selected day. "
                           "Includes profit and loss graphs and detailed information about all performed deals and open positions.",
             allocator), allocator);
    response.AddMember("type", REPORT_DAILY_GROUP_TYPE, allocator);
}

extern "C" void DestroyReport() {}

extern "C" void CreateReport(rapidjson::Value& request,
                             rapidjson::Value& response,
                             rapidjson::Document::AllocatorType& allocator,
                             CServerInterface* server) {
    std::string group_mask;
    int from;
    int to;
    int from_two_weeks_ago;
    if (request.HasMember("group") && request["group"].IsString()) {
        group_mask = request["group"].GetString();
    }
    if (request.HasMember("from") && request["from"].IsNumber()) {
        from = request["from"].GetInt();
        from_two_weeks_ago = utils::CalculateTimestampForTwoWeeksAgo(from);
    }
    if (request.HasMember("to") && request["to"].IsNumber()) {
        to = request["to"].GetInt();
    }

    std::vector<TradeRecord> close_trades_vector;
    std::vector<TradeRecord> open_trades_vector;
    std::vector<GroupRecord> groups_vector;
    std::vector<UsdConvertedTrade> usd_converted_close_trades_vector;
    std::vector<UsdConvertedTrade> usd_converted_open_trades_vector;

    try {
        server->GetCloseTradesByGroup(group_mask, from_two_weeks_ago, to, &close_trades_vector);
        server->GetOpenTradesByGroup(group_mask, from_two_weeks_ago, to, &open_trades_vector);
        server->GetAllGroups(&groups_vector);

        for (auto& close_trade : close_trades_vector) {
            AccountRecord account;
            double multiplier;

            server->GetAccountByLogin(close_trade.login, &account);

            for (const auto& group : groups_vector) {
                if (group.group == account.group) {
                    double usd_profit = 0.00;

                    UsdConvertedTrade converted_close_trade;

                    if (group.currency == "USD") {
                        usd_profit = close_trade.profit;
                    } else {
                        server->CalculateConvertRateByCurrency(group.currency, "USD", close_trade.cmd, &multiplier);
                        usd_profit = close_trade.profit * multiplier;
                    }

                    converted_close_trade.usd_profit = usd_profit;
                    converted_close_trade.close_time = close_trade.close_time;

                    usd_converted_close_trades_vector.emplace_back(converted_close_trade);
                }
            }
        }

        for (const auto& open_trade : open_trades_vector) {
            AccountRecord account;
            double multiplier;

            server->GetAccountByLogin(open_trade.login, &account);

            for (const auto& group : groups_vector) {
                if (group.group == account.group) {
                    double usd_profit = 0.00;

                    UsdConvertedTrade converted_close_trade;

                    if (group.currency == "USD") {
                        usd_profit = open_trade.profit;
                    } else {
                        server->CalculateConvertRateByCurrency(group.currency, "USD", open_trade.cmd, &multiplier);
                        usd_profit = open_trade.profit * multiplier;
                    }

                    converted_close_trade.usd_profit = usd_profit;
                    converted_close_trade.close_time = open_trade.close_time;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[DailyTradesReportInterface]: " << e.what() << std::endl;
    }

    // Лямбда подготавливающая значения double для вставки в AST (округление до 2-х знаков)
    auto format_double_for_AST = [](double value) -> std::string {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << value;
        return oss.str();
    };

    // Profit / Lose chart
    const JSONArray pnl_chart_data = utils::CreatePnlChartData(usd_converted_close_trades_vector);

    Node pnl_chart_node = ResponsiveContainer({
        LineChart({
            XAxis({}, props({{"dataKey", "day"}})),
            YAxis(),
            Tooltip(),
            Legend(),

            Line({}, props({
                {"type", "monotone"},
                {"dataKey", "profit"},
                {"stroke", "#4A90E2"}
            })),

            Line({}, props({
                {"type", "monotone"},
                {"dataKey", "loss"},
                {"stroke", "#7ED321"}
            })),

            Line({}, props({
                {"type", "monotone"},
                {"dataKey", "profit/loss"},
                {"stroke", "#F5A623"}
            }))
        }, props({
            {"data", pnl_chart_data}
        }))
    }, props({
        {"width", "100%"},
        {"height", 300.0}
    }));

    // Clients trades count chart
    const JSONArray trades_count_chart_data = utils::CreateTradesCountChartData(close_trades_vector);

    Node trades_count_chart_node = ResponsiveContainer({
        LineChart({
            XAxis({}, props({{"dataKey", "day"}})),
            YAxis(),
            Tooltip(),
            Legend(),

            Line({}, props({
                {"type", "monotone"},
                {"dataKey", "profit"},
                {"stroke", "#4A90E2"}
            })),

            Line({}, props({
                {"type", "monotone"},
                {"dataKey", "loss"},
                {"stroke", "#7ED321"}
            })),
        }, props({
            {"data", trades_count_chart_data}
        }))
    }, props({
        {"width", "100%"},
        {"height", 300.0}
    }));

    // Top close profit orders table
    std::vector<TradeRecord> top_close_profit_orders_vector = utils::CreateTopProfitOrdersVector(close_trades_vector);
    TableBuilder top_close_profit_orders_table_builder("TopCloseProfitOrdersTable");

    top_close_profit_orders_table_builder.SetIdColumn("order");
    top_close_profit_orders_table_builder.SetOrderBy("order", "DESC");
    top_close_profit_orders_table_builder.EnableRefreshButton(false);
    top_close_profit_orders_table_builder.EnableBookmarksButton(false);
    top_close_profit_orders_table_builder.EnableExportButton(true);

    top_close_profit_orders_table_builder.AddColumn({"order", "ORDER"});
    top_close_profit_orders_table_builder.AddColumn({"login", "LOGIN"});
    top_close_profit_orders_table_builder.AddColumn({"name", "NAME"});
    top_close_profit_orders_table_builder.AddColumn({"symbol", "SYMBOL"});
    top_close_profit_orders_table_builder.AddColumn({"group", "GROUP"});
    top_close_profit_orders_table_builder.AddColumn({"type", "TYPE"});
    top_close_profit_orders_table_builder.AddColumn({"volume", "VOLUME"});
    top_close_profit_orders_table_builder.AddColumn({"close_price", "CLOSE_PRICE"});
    top_close_profit_orders_table_builder.AddColumn({"storage", "SWAP"});
    top_close_profit_orders_table_builder.AddColumn({"profit", "AMOUNT"});

    for (const auto& trade : top_close_profit_orders_vector) {
        AccountRecord account;

        try {
            server->GetAccountByLogin(trade.login, &account);
        } catch (const std::exception& e) {
            std::cerr << "[DailyTradesReportInterface]: " << e.what() << std::endl;
        }

        top_close_profit_orders_table_builder.AddRow({
            {"order", std::to_string(trade.order)},
            {"login", std::to_string(trade.login)},
            {"name", account.name},
            {"symbol", trade.symbol},
            {"group", account.group},
            {"type", trade.cmd == 0 ? "buy" : "sell"},
            {"volume", std::to_string(trade.volume)},
            {"close_price", format_double_for_AST(trade.close_price)},
            {"storage", format_double_for_AST(trade.storage)},
            {"profit", format_double_for_AST(trade.profit)},
        });
    }

    const JSONObject top_close_profit_orders_table_props = top_close_profit_orders_table_builder.CreateTableProps();
    const Node top_close_profit_orders_table_node = Table({}, top_close_profit_orders_table_props);

    // Top close loss orders table
    std::vector<TradeRecord> top_close_loss_orders_vector = utils::CreateTopLossOrdersVector(close_trades_vector);
    TableBuilder top_close_loss_orders_table_builder("TopCloseLossOrdersTable");

    top_close_loss_orders_table_builder.SetIdColumn("order");
    top_close_loss_orders_table_builder.SetOrderBy("order", "DESC");
    top_close_loss_orders_table_builder.EnableRefreshButton(false);
    top_close_loss_orders_table_builder.EnableBookmarksButton(false);
    top_close_loss_orders_table_builder.EnableExportButton(true);

    top_close_loss_orders_table_builder.AddColumn({"order", "ORDER"});
    top_close_loss_orders_table_builder.AddColumn({"login", "LOGIN"});
    top_close_loss_orders_table_builder.AddColumn({"name", "NAME"});
    top_close_loss_orders_table_builder.AddColumn({"symbol", "SYMBOL"});
    top_close_loss_orders_table_builder.AddColumn({"group", "GROUP"});
    top_close_loss_orders_table_builder.AddColumn({"type", "TYPE"});
    top_close_loss_orders_table_builder.AddColumn({"volume", "VOLUME"});
    top_close_loss_orders_table_builder.AddColumn({"close_price", "CLOSE_PRICE"});
    top_close_loss_orders_table_builder.AddColumn({"storage", "SWAP"});
    top_close_loss_orders_table_builder.AddColumn({"profit", "AMOUNT"});

    for (const auto& trade : top_close_loss_orders_vector) {
        AccountRecord account;

        try {
            server->GetAccountByLogin(trade.login, &account);
        } catch (const std::exception& e) {
            std::cerr << "[DailyTradesReportInterface]: " << e.what() << std::endl;
        }

        top_close_loss_orders_table_builder.AddRow({
            {"order", std::to_string(trade.order)},
            {"login", std::to_string(trade.login)},
            {"name", account.name},
            {"symbol", trade.symbol},
            {"group", account.group},
            {"type", trade.cmd == 0 ? "buy" : "sell"},
            {"volume", std::to_string(trade.volume)},
            {"close_price", format_double_for_AST(trade.close_price)},
            {"storage", format_double_for_AST(trade.storage)},
            {"profit", format_double_for_AST(trade.profit)},
        });
    }

    const JSONObject top_close_loss_orders_table_props = top_close_loss_orders_table_builder.CreateTableProps();
    const Node top_close_loss_orders_table_node = Table({}, top_close_loss_orders_table_props);

    // Total current positions chart
    const JSONArray current_positions_chart_data = utils::CreateOpenPositionsPieChartData(usd_converted_open_trades_vector);

    Node current_positions_pie_chart = ResponsiveContainer({
        PieChart({
            Tooltip(),
            Legend(),
            Pie({
                Cell({}, props({ {"fill", "#4A90E2"} })),   // profit
                Cell({}, props({ {"fill", "#7ED321"} })),   // lose
            }, props({
                {"dataKey", "value"},
                {"nameKey", "name"},
                {"data", current_positions_chart_data},
                {"cx", "50%"},
                {"cy", "50%"},
                {"outerRadius", 100.0},
                {"label", true}
            }))
        })
    }, props({
        {"width", "100%"},
        {"height", 300.0}
    }));

    // Top open profit orders table
    std::vector<TradeRecord> top_open_profit_orders_vector = utils::CreateTopProfitOrdersVector(open_trades_vector);
    TableBuilder top_open_profit_orders_table_builder("TopOpenProfitOrdersTable");

    top_open_profit_orders_table_builder.SetIdColumn("order");
    top_open_profit_orders_table_builder.SetOrderBy("order", "DESC");
    top_open_profit_orders_table_builder.EnableRefreshButton(false);
    top_open_profit_orders_table_builder.EnableBookmarksButton(false);
    top_open_profit_orders_table_builder.EnableExportButton(true);

    top_open_profit_orders_table_builder.AddColumn({"order", "ORDER"});
    top_open_profit_orders_table_builder.AddColumn({"login", "LOGIN"});
    top_open_profit_orders_table_builder.AddColumn({"name", "NAME"});
    top_open_profit_orders_table_builder.AddColumn({"symbol", "SYMBOL"});
    top_open_profit_orders_table_builder.AddColumn({"group", "GROUP"});
    top_open_profit_orders_table_builder.AddColumn({"type", "TYPE"});
    top_open_profit_orders_table_builder.AddColumn({"volume", "VOLUME"});
    top_open_profit_orders_table_builder.AddColumn({"close_price", "CLOSE_PRICE"});
    top_open_profit_orders_table_builder.AddColumn({"storage", "SWAP"});
    top_open_profit_orders_table_builder.AddColumn({"profit", "AMOUNT"});

    for (const auto& trade : top_open_profit_orders_vector) {
        AccountRecord account;

        try {
            server->GetAccountByLogin(trade.login, &account);
        } catch (const std::exception& e) {
            std::cerr << "[DailyTradesReportInterface]: " << e.what() << std::endl;
        }

        top_open_profit_orders_table_builder.AddRow({
            {"order", std::to_string(trade.order)},
            {"login", std::to_string(trade.login)},
            {"name", account.name},
            {"symbol", trade.symbol},
            {"group", account.group},
            {"type", trade.cmd == 0 ? "buy" : "sell"},
            {"volume", std::to_string(trade.volume)},
            {"close_price", format_double_for_AST(trade.close_price)},
            {"storage", format_double_for_AST(trade.storage)},
            {"profit", format_double_for_AST(trade.profit)},
        });
    }

    const JSONObject top_open_profit_orders_table_props = top_open_profit_orders_table_builder.CreateTableProps();
    const Node top_open_profit_orders_table_node = Table({}, top_open_profit_orders_table_props);

    // Top open loss orders table
    std::vector<TradeRecord> top_open_loss_orders_vector = utils::CreateTopLossOrdersVector(open_trades_vector);
    TableBuilder top_open_loss_orders_table_builder("TopOpenLossOrdersTable");

    top_open_loss_orders_table_builder.SetIdColumn("order");
    top_open_loss_orders_table_builder.SetOrderBy("order", "DESC");
    top_open_loss_orders_table_builder.EnableRefreshButton(false);
    top_open_loss_orders_table_builder.EnableBookmarksButton(false);
    top_open_loss_orders_table_builder.EnableExportButton(true);

    top_open_loss_orders_table_builder.AddColumn({"order", "ORDER"});
    top_open_loss_orders_table_builder.AddColumn({"login", "LOGIN"});
    top_open_loss_orders_table_builder.AddColumn({"name", "NAME"});
    top_open_loss_orders_table_builder.AddColumn({"symbol", "SYMBOL"});
    top_open_loss_orders_table_builder.AddColumn({"group", "GROUP"});
    top_open_loss_orders_table_builder.AddColumn({"type", "TYPE"});
    top_open_loss_orders_table_builder.AddColumn({"volume", "VOLUME"});
    top_open_loss_orders_table_builder.AddColumn({"close_price", "CLOSE_PRICE"});
    top_open_loss_orders_table_builder.AddColumn({"storage", "SWAP"});
    top_open_loss_orders_table_builder.AddColumn({"profit", "AMOUNT"});

    for (const auto& trade : top_open_loss_orders_vector) {
        AccountRecord account;

        try {
            server->GetAccountByLogin(trade.login, &account);
        } catch (const std::exception& e) {
            std::cerr << "[DailyTradesReportInterface]: " << e.what() << std::endl;
        }

        top_open_loss_orders_table_builder.AddRow({
            {"order", std::to_string(trade.order)},
            {"login", std::to_string(trade.login)},
            {"name", account.name},
            {"symbol", trade.symbol},
            {"group", account.group},
            {"type", trade.cmd == 0 ? "buy" : "sell"},
            {"volume", std::to_string(trade.volume)},
            {"close_price", format_double_for_AST(trade.close_price)},
            {"storage", format_double_for_AST(trade.storage)},
            {"profit", format_double_for_AST(trade.profit)},
        });
    }

    const JSONObject top_open_loss_orders_table_props = top_open_loss_orders_table_builder.CreateTableProps();
    const Node top_open_loss_orders_table_node = Table({}, top_open_loss_orders_table_props);

    // Total report
    const Node report = Column({
        h1({text("Daily Trades Report")}),
        h2({text("Profit and Loss of Clients, USD")}),
        pnl_chart_node,
        h2({text("Client Trades Count")}),
        trades_count_chart_node,
        h2({text("Top Close Profit Orders")}),
        top_close_profit_orders_table_node,
        h2({text("Top Close Loss Orders")}),
        top_close_loss_orders_table_node,
        h2({text("Total Profit/Loss of Current Client Positions, USD (%)")}),
        current_positions_pie_chart,
        h2({text("Top Open Profit Orders")}),
        top_open_profit_orders_table_node,
        h2({text("Top Open Loss Orders")}),
        top_open_loss_orders_table_node
    });

    utils::CreateUI(report, response, allocator);
}