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

        for (auto close_trade : close_trades_vector) {
            AccountRecord account;
            double multiplier;

            server->GetAccountByLogin(close_trade.login, &account);

            for (const auto& group : groups_vector) {
                if (group.group == account.group) {
                    UsdConvertedTrade converted_close_trade;

                    server->CalculateConvertRateByCurrency(group.currency, "USD", close_trade.cmd, &multiplier);

                    converted_close_trade.close_time = close_trade.close_time;
                    converted_close_trade.usd_profit = close_trade.profit * multiplier;

                    usd_converted_close_trades_vector.emplace_back(converted_close_trade);
                }
            }
        }

        for (auto open_trade : open_trades_vector) {
            AccountRecord account;
            double multiplier;

            server->GetAccountByLogin(open_trade.login, &account);

            for (const auto& group : groups_vector) {
                if (group.group == account.group) {
                    UsdConvertedTrade converted_open_trade;

                    server->CalculateConvertRateByCurrency(group.currency, "USD", open_trade.cmd, &multiplier);

                    converted_open_trade.close_time = open_trade.close_time;
                    converted_open_trade.usd_profit = open_trade.profit * multiplier;

                    usd_converted_open_trades_vector.emplace_back(converted_open_trade);
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

    // v.1
    // auto create_top_close_profit_orders_table = [&](const std::vector<TradeRecord>& trades) -> Node {
    //     std::vector<Node> thead_rows;
    //     std::vector<Node> tbody_rows;
    //     std::vector<Node> tfoot_rows;
    //
    //     // Thead
    //     thead_rows.push_back(tr({
    //         th({div({text("Order")})}),
    //         th({div({text("Login")})}),
    //         th({div({text("Name")})}),
    //         th({div({text("Symbol")})}),
    //         th({div({text("Group")})}),
    //         th({div({text("Type")})}),
    //         th({div({text("Volume")})}),
    //         th({div({text("Close Price")})}),
    //         th({div({text("Storage")})}),
    //         th({div({text("Profit")})})
    //     }));
    //
    //     // Tbody
    //     for (const auto& trade : trades) {
    //         AccountRecord account;
    //
    //         try {
    //             server->GetAccountByLogin(trade.login, &account);
    //         } catch (const std::exception& e) {
    //             std::cerr << "[DailyTradesReportInterface]: " << e.what() << std::endl;
    //         }
    //
    //         tbody_rows.push_back(tr({
    //             td({div({text(std::to_string(trade.order))})}),
    //             td({div({text(std::to_string(trade.login))})}),
    //             td({div({text(account.name)})}),
    //             td({div({text(trade.symbol)})}),
    //             td({div({text(account.group)})}),
    //             td({div({text(trade.cmd == 0 ? "buy" : "sell")})}),
    //             td({div({text(std::to_string(trade.volume))})}),
    //             td({div({text(format_double_for_AST(trade.close_price))})}),
    //             td({div({text(format_double_for_AST(trade.storage))})}),
    //             td({div({text(format_double_for_AST(trade.profit))})}),
    //         }));
    //     }
    //
    //     return table({
    //         thead(thead_rows),
    //         tbody(tbody_rows),
    //         tfoot(tfoot_rows),
    //     }, props({{"className", "table"}}));
    // };

    // v.2
    TableBuilder table_builder("DailyTradesReport");

    table_builder.SetIdColumn("order");
    table_builder.SetOrderBy("order", "DESC");
    table_builder.EnableRefreshButton(false);
    table_builder.EnableBookmarksButton(false);
    table_builder.EnableExportButton(true);

    table_builder.AddColumn({"order", "ORDER"});
    table_builder.AddColumn({"login", "LOGIN"});
    table_builder.AddColumn({"name", "NAME"});
    table_builder.AddColumn({"symbol", "SYMBOL"});
    table_builder.AddColumn({"group", "GROUP"});
    table_builder.AddColumn({"type", "TYPE"});
    table_builder.AddColumn({"volume", "VOLUME"});
    table_builder.AddColumn({"close_price", "CLOSE_PRICE"});
    table_builder.AddColumn({"storage", "SWAP"});
    table_builder.AddColumn({"profit", "AMOUNT"});

    for (const auto& trade : top_close_profit_orders_vector) {
        AccountRecord account;

        try {
            server->GetAccountByLogin(trade.login, &account);
        } catch (const std::exception& e) {
            std::cerr << "[DailyTradesReportInterface]: " << e.what() << std::endl;
        }

        table_builder.AddRow({
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

    const JSONObject top_close_profit_orders_table_props = table_builder.CreateTableProps();
    const Node top_close_profit_orders_table_node = Table({}, top_close_profit_orders_table_props);

    // Top close loss orders table
    std::vector<TradeRecord> top_close_loss_orders_vector = utils::CreateTopLossOrdersVector(close_trades_vector);

    auto create_top_close_loss_orders_table = [&](const std::vector<TradeRecord>& trades) -> Node {
        std::vector<Node> thead_rows;
        std::vector<Node> tbody_rows;
        std::vector<Node> tfoot_rows;

        // Thead
        thead_rows.push_back(tr({
            th({div({text("Order")})}),
            th({div({text("Login")})}),
            th({div({text("Name")})}),
            th({div({text("Symbol")})}),
            th({div({text("Group")})}),
            th({div({text("Type")})}),
            th({div({text("Volume")})}),
            th({div({text("Close Price")})}),
            th({div({text("Swap")})}),
            th({div({text("Profit")})})
        }));

        // Tbody
        for (const auto& trade : trades) {
            AccountRecord account;

            try {
                server->GetAccountByLogin(trade.login, &account);
            } catch (const std::exception& e) {
                std::cerr << "[DailyTradesReportInterface]: " << e.what() << std::endl;
            }

            tbody_rows.push_back(tr({
                td({div({text(std::to_string(trade.order))})}),
                td({div({text(std::to_string(trade.login))})}),
                td({div({text(account.name)})}),
                td({div({text(trade.symbol)})}),
                td({div({text(account.group)})}),
                td({div({text(trade.cmd == 0 ? "buy" : "sell")})}),
                td({div({text(std::to_string(trade.volume))})}),
                td({div({text(format_double_for_AST(trade.close_price))})}),
                td({div({text(format_double_for_AST(trade.storage))})}),
                td({div({text(format_double_for_AST(trade.profit))})}),
            }));
        }

        return table({
            thead(thead_rows),
            tbody(tbody_rows),
            tfoot(tfoot_rows),
        }, props({{"className", "table"}}));
    };

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

    auto create_top_open_profit_orders_table = [&](const std::vector<TradeRecord>& trades) -> Node {
        std::vector<Node> thead_rows;
        std::vector<Node> tbody_rows;
        std::vector<Node> tfoot_rows;

        // Thead
        thead_rows.push_back(tr({
            th({div({text("Order")})}),
            th({div({text("Login")})}),
            th({div({text("Name")})}),
            th({div({text("Symbol")})}),
            th({div({text("Group")})}),
            th({div({text("Type")})}),
            th({div({text("Volume")})}),
            th({div({text("Open Price")})}),
            th({div({text("S / L")})}),
            th({div({text("T / P")})}),
            th({div({text("Market Price")})}),
            th({div({text("Storage")})}),
            th({div({text("Commission")})}),
            th({div({text("Profit")})})
        }));

        // Tbody
        for (const auto& trade : trades) {
            AccountRecord account;

            try {
                server->GetAccountByLogin(trade.login, &account);
            } catch (const std::exception& e) {
                std::cerr << "[DailyTradesReportInterface]: " << e.what() << std::endl;
            }

            tbody_rows.push_back(tr({
                td({div({text(std::to_string(trade.order))})}),
                td({div({text(std::to_string(trade.login))})}),
                td({div({text(account.name)})}),
                td({div({text(trade.symbol)})}),
                td({div({text(account.group)})}),
                td({div({text(trade.cmd == 0 ? "buy" : "sell")})}),
                td({div({text(std::to_string(trade.volume))})}),
                td({div({text(format_double_for_AST(trade.open_price))})}),
                td({div({text(std::to_string(trade.sl))})}),
                td({div({text(std::to_string(trade.tp))})}),
                td({div({text("Market price")})}),
                td({div({text(format_double_for_AST(trade.storage))})}),
                td({div({text(format_double_for_AST(trade.commission))})}),
                td({div({text(format_double_for_AST(trade.profit))})}),
            }));
        }

        return table({
            thead(thead_rows),
            tbody(tbody_rows),
            tfoot(tfoot_rows),
        }, props({{"className", "table"}}));
    };

    // Top open loss orders table
    std::vector<TradeRecord> top_open_loss_orders_vector = utils::CreateTopLossOrdersVector(open_trades_vector);

    auto create_top_open_loss_orders_table = [&](const std::vector<TradeRecord>& trades) -> Node {
        std::vector<Node> thead_rows;
        std::vector<Node> tbody_rows;
        std::vector<Node> tfoot_rows;

        // Thead
        thead_rows.push_back(tr({
            th({div({text("Order")})}),
            th({div({text("Login")})}),
            th({div({text("Name")})}),
            th({div({text("Symbol")})}),
            th({div({text("Group")})}),
            th({div({text("Type")})}),
            th({div({text("Volume")})}),
            th({div({text("Open Price")})}),
            th({div({text("S / L")})}),
            th({div({text("T / P")})}),
            th({div({text("Market Price")})}),
            th({div({text("Storage")})}),
            th({div({text("Commission")})}),
            th({div({text("Profit")})})
        }));

        // Tbody
        for (const auto& trade : trades) {
            AccountRecord account;

            try {
                server->GetAccountByLogin(trade.login, &account);
            } catch (const std::exception& e) {
                std::cerr << "[DailyTradesReportInterface]: " << e.what() << std::endl;
            }

            tbody_rows.push_back(tr({
                td({div({text(std::to_string(trade.order))})}),
                td({div({text(std::to_string(trade.login))})}),
                td({div({text(account.name)})}),
                td({div({text(trade.symbol)})}),
                td({div({text(account.group)})}),
                td({div({text(trade.cmd == 0 ? "buy" : "sell")})}),
                td({div({text(std::to_string(trade.volume))})}),
                td({div({text(format_double_for_AST(trade.open_price))})}),
                td({div({text(std::to_string(trade.sl))})}),
                td({div({text(std::to_string(trade.tp))})}),
                td({div({text("Market price")})}),
                td({div({text(format_double_for_AST(trade.storage))})}),
                td({div({text(format_double_for_AST(trade.commission))})}),
                td({div({text(format_double_for_AST(trade.profit))})}),
            }));
        }

        return table({
            thead(thead_rows),
            tbody(tbody_rows),
            tfoot(tfoot_rows),
        }, props({{"className", "table"}}));
    };

    // Total report
    const Node report = div({
        h1({text("Daily Trades Report")}),
        h2({text("Profit and Loss of Clients, USD")}),
        pnl_chart_node,
        h2({text("Client Trades Count")}),
        trades_count_chart_node,
        h2({text("Top Close Profit Orders")}),
        // create_top_close_profit_orders_table(top_close_profit_orders_vector), // v.1
        top_close_profit_orders_table_node,
        h2({text("Top Close Loss Orders")}),
        create_top_close_loss_orders_table(top_close_loss_orders_vector),
        h2({text("Total Profit/Loss of Current Client Positions, USD (%)")}),
        current_positions_pie_chart,
        h2({text("Top Open Profit Orders")}),
        create_top_open_profit_orders_table(top_open_profit_orders_vector),
        h2({text("Top Open Loss Orders")}),
        create_top_open_loss_orders_table(top_open_loss_orders_vector),
    });

    utils::CreateUI(report, response, allocator);
}