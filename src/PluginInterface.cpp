#include "PluginInterface.h"

#include <iomanip>

extern "C" void AboutReport(rapidjson::Value&                   request,
                            rapidjson::Value&                   response,
                            rapidjson::Document::AllocatorType& allocator,
                            ReportServerInterface*              server) {
    response.AddMember("version", 1, allocator);
    response.AddMember("name", Value().SetString("Daily Trades report", allocator), allocator);
    response.AddMember(
        "description",
        Value().SetString("Trading operations of selected trader groups for the selected day. "
                          "Includes profit and loss graphs and detailed information about all "
                          "performed deals and open positions.",
                          allocator),
        allocator);
    response.AddMember("type", static_cast<int>(ReportType::DailyGroup), allocator);
    response.AddMember("key", Value().SetString("DAILY_TRADES_REPORT", allocator), allocator);
}

extern "C" void DestroyReport() {}

extern "C" void CreateReport(rapidjson::Value&                   request,
                             rapidjson::Value&                   response,
                             rapidjson::Document::AllocatorType& allocator,
                             ReportServerInterface*              server) {
    std::string group_mask;
    int         from;
    int         to;
    int         from_two_weeks_ago;

    if (request.HasMember("group") && request["group"].IsString()) {
        group_mask = request["group"].GetString();
    }
    if (request.HasMember("from") && request["from"].IsNumber()) {
        from               = request["from"].GetInt();
        from_two_weeks_ago = utils::CalculateTimestampForTwoWeeksAgo(from);
    }
    if (request.HasMember("to") && request["to"].IsNumber()) {
        to = request["to"].GetInt();
    }

    std::vector<ReportTradeRecord> close_trades_vector;
    std::vector<ReportTradeRecord> open_trades_vector;
    std::vector<ReportGroupRecord> groups_vector;
    std::vector<ConvertedTrade>    usd_converted_close_trades_vector;
    std::vector<ConvertedTrade>    usd_converted_open_trades_vector;

    try {
        server->GetCloseTradesByGroup(group_mask, from_two_weeks_ago, to, &close_trades_vector);
        server->GetOpenTradesByGroup(group_mask, from_two_weeks_ago, to, &open_trades_vector);
        server->GetAllGroups(&groups_vector);

        for (auto& close_trade : close_trades_vector) {
            ReportAccountRecord account_record;

            try {
                server->GetAccountByLogin(close_trade.login, &account_record);
            } catch (const std::exception& e) {
                std::cerr << "[DailyTradesReportInterface]: " << e.what() << std::endl;
            }

            for (const auto& group : groups_vector) {
                if (group.group == account_record.group) {
                    double multiplier = 1;
                    double profit     = 0.00;

                    ConvertedTrade converted_close_trade{};

                    // Conversion disabled
                    // if (group.currency != "USD") {
                    //     try {
                    //         server->CalculateConvertRateByCurrency(
                    //             group.currency,
                    //             "USD",
                    //             static_cast<int>(close_trade.cmd),
                    //             &multiplier);
                    //     } catch (const std::exception& e) {
                    //         std::cerr << "[DailyTradesReportInterface]: " << e.what() <<
                    //         std::endl;
                    //     }
                    // }

                    converted_close_trade.profit     = profit * multiplier;
                    converted_close_trade.close_time = close_trade.close_time;

                    usd_converted_close_trades_vector.emplace_back(converted_close_trade);
                }
            }
        }

        for (const auto& open_trade : open_trades_vector) {
            ReportAccountRecord account_record;

            try {
                server->GetAccountByLogin(open_trade.login, &account_record);
            } catch (const std::exception& e) {
                std::cerr << "[DailyTradesReportInterface]: " << e.what() << std::endl;
            }

            for (const auto& group : groups_vector) {
                if (group.group == account_record.group) {
                    double multiplier = 1;
                    double profit     = 0.00;

                    ConvertedTrade converted_close_trade{};

                    // if (group.currency != "USD") {
                    //     try {
                    //         server->CalculateConvertRateByCurrency(group.currency,
                    //                                                "USD",
                    //                                                static_cast<int>(open_trade.cmd),
                    //                                                &multiplier);
                    //     } catch (const std::exception& e) {
                    //         std::cerr << "[DailyTradesReportInterface]: " << e.what() <<
                    //         std::endl;
                    //     }
                    // }

                    converted_close_trade.profit     = profit * multiplier;
                    converted_close_trade.close_time = open_trade.close_time;

                    usd_converted_open_trades_vector.emplace_back(converted_close_trade);
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[DailyTradesReportInterface]: " << e.what() << std::endl;
    }

    // Profit / Lose chart
    const JSONArray pnl_chart_data = utils::CreatePnlChartData(usd_converted_close_trades_vector);

    Node pnl_chart_node = ResponsiveContainer(
        {LineChart(
            {XAxis({}, props({{"dataKey", "day"}})),
             YAxis(),
             Tooltip(),
             Legend(),

             Line({}, props({{"type", "monotone"}, {"dataKey", "profit"}, {"stroke", "#4A90E2"}})),
             Line({}, props({{"type", "monotone"}, {"dataKey", "loss"}, {"stroke", "#7ED321"}})),
             Line(
                 {},
                 props({{"type", "monotone"}, {"dataKey", "profit/loss"}, {"stroke", "#F5A623"}}))},
            props({{"data", pnl_chart_data}}))},
        props({{"width", "100%"}, {"height", 300.0}}));

    // Clients trades count chart
    const JSONArray trades_count_chart_data =
        utils::CreateTradesCountChartData(close_trades_vector);

    Node trades_count_chart_node = ResponsiveContainer(
        {LineChart(
            {
                XAxis({}, props({{"dataKey", "day"}})),
                YAxis(),
                Tooltip(),
                Legend(),

                Line({},
                     props({{"type", "monotone"}, {"dataKey", "profit"}, {"stroke", "#4A90E2"}})),
                Line({}, props({{"type", "monotone"}, {"dataKey", "loss"}, {"stroke", "#7ED321"}})),
            },
            props({{"data", trades_count_chart_data}}))},
        props({{"width", "100%"}, {"height", 300.0}}));

    // Table filters
    FilterConfig search_filter;
    search_filter.type = FilterType::Search;

    FilterConfig date_time_filter;
    date_time_filter.type = FilterType::DateTime;

    FilterConfig group_select_filter;
    group_select_filter.type = FilterType::Select;
    for (const auto& group : groups_vector) {
        group_select_filter.options.push_back({group.group, group.group});
    }

    // Top close profit orders table
    std::vector<ReportTradeRecord> top_close_profit_orders_vector =
        utils::CreateTopProfitOrdersVector(close_trades_vector);
    TableBuilder top_close_profit_orders_table_builder("TopCloseProfitOrdersTable");

    top_close_profit_orders_table_builder.SetIdColumn("order");
    top_close_profit_orders_table_builder.SetOrderBy("profit", "DESC");
    top_close_profit_orders_table_builder.EnableAutoSave(false);
    top_close_profit_orders_table_builder.EnableRefreshButton(false);
    top_close_profit_orders_table_builder.EnableBookmarksButton(false);
    top_close_profit_orders_table_builder.EnableExportButton(true);

    top_close_profit_orders_table_builder.AddColumn({"order", "ORDER", 1, search_filter});
    top_close_profit_orders_table_builder.AddColumn({"login", "LOGIN", 2, search_filter});
    top_close_profit_orders_table_builder.AddColumn({"name", "NAME", 3, search_filter});
    top_close_profit_orders_table_builder.AddColumn({"symbol", "SYMBOL", 4, search_filter});
    top_close_profit_orders_table_builder.AddColumn({"group", "GROUP", 5, group_select_filter});
    top_close_profit_orders_table_builder.AddColumn({"type", "TYPE", 6});
    top_close_profit_orders_table_builder.AddColumn({"volume", "VOLUME", 7, search_filter});
    top_close_profit_orders_table_builder.AddColumn(
        {"close_price", "CLOSE_PRICE", 8, search_filter});
    top_close_profit_orders_table_builder.AddColumn({"storage", "SWAP", 9, search_filter});
    top_close_profit_orders_table_builder.AddColumn({"profit", "AMOUNT", 10, search_filter});
    top_close_profit_orders_table_builder.AddColumn({"currency", "CURRENCY", 10, search_filter});

    for (const auto& trade : top_close_profit_orders_vector) {
        ReportAccountRecord account_record;
        std::string currency = utils::GetGroupCurrencyByName(groups_vector, account_record.group);

        try {
            server->GetAccountByLogin(trade.login, &account_record);
        } catch (const std::exception& e) {
            std::cerr << "[DailyTradesReportInterface]: " << e.what() << std::endl;
        }

        top_close_profit_orders_table_builder.AddRow(
            {utils::TruncateDouble(trade.order, 0),
             utils::TruncateDouble(trade.login, 0),
             account_record.name,
             trade.symbol,
             account_record.group,
             utils::ConvertCmdToString(static_cast<int>(trade.cmd)),
             utils::TruncateDouble(trade.volume / 100.0, 2),
             utils::TruncateDouble(trade.close_price, 2),
             utils::TruncateDouble(trade.storage, 2),
             utils::TruncateDouble(trade.profit, 2),
             currency});
    }

    const JSONObject top_close_profit_orders_table_props =
        top_close_profit_orders_table_builder.CreateTableProps();
    const Node top_close_profit_orders_table_node = Table({}, top_close_profit_orders_table_props);

    // Top close loss orders table
    std::vector<ReportTradeRecord> top_close_loss_orders_vector =
        utils::CreateTopLossOrdersVector(close_trades_vector);
    TableBuilder top_close_loss_orders_table_builder("TopCloseLossOrdersTable");

    top_close_loss_orders_table_builder.SetIdColumn("order");
    top_close_loss_orders_table_builder.SetOrderBy("profit", "ASC");
    top_close_loss_orders_table_builder.EnableAutoSave(false);
    top_close_loss_orders_table_builder.EnableRefreshButton(false);
    top_close_loss_orders_table_builder.EnableBookmarksButton(false);
    top_close_loss_orders_table_builder.EnableExportButton(true);

    top_close_loss_orders_table_builder.AddColumn({"order", "ORDER", 1, search_filter});
    top_close_loss_orders_table_builder.AddColumn({"login", "LOGIN", 2, search_filter});
    top_close_loss_orders_table_builder.AddColumn({"name", "NAME", 3, search_filter});
    top_close_loss_orders_table_builder.AddColumn({"symbol", "SYMBOL", 4, search_filter});
    top_close_loss_orders_table_builder.AddColumn({"group", "GROUP", 5, group_select_filter});
    top_close_loss_orders_table_builder.AddColumn({"type", "TYPE", 6});
    top_close_loss_orders_table_builder.AddColumn({"volume", "VOLUME", 7, search_filter});
    top_close_loss_orders_table_builder.AddColumn({"close_price", "CLOSE_PRICE", 8, search_filter});
    top_close_loss_orders_table_builder.AddColumn({"storage", "SWAP", 9, search_filter});
    top_close_loss_orders_table_builder.AddColumn({"profit", "AMOUNT", 10, search_filter});
    top_close_loss_orders_table_builder.AddColumn({"currency", "CURRENCY", 11, search_filter});

    for (const auto& trade : top_close_loss_orders_vector) {
        ReportAccountRecord account_record;
        std::string currency = utils::GetGroupCurrencyByName(groups_vector, account_record.group);

        try {
            server->GetAccountByLogin(trade.login, &account_record);
        } catch (const std::exception& e) {
            std::cerr << "[DailyTradesReportInterface]: " << e.what() << std::endl;
        }

        top_close_loss_orders_table_builder.AddRow(
            {utils::TruncateDouble(trade.order, 0),
             utils::TruncateDouble(trade.login, 0),
             account_record.name,
             trade.symbol,
             account_record.group,
             utils::ConvertCmdToString(static_cast<int>(trade.cmd)),
             utils::TruncateDouble(trade.volume / 100.0, 2),
             utils::TruncateDouble(trade.close_price, 2),
             utils::TruncateDouble(trade.storage, 2),
             utils::TruncateDouble(trade.profit, 2),
             currency});
    }

    const JSONObject top_close_loss_orders_table_props =
        top_close_loss_orders_table_builder.CreateTableProps();
    const Node top_close_loss_orders_table_node = Table({}, top_close_loss_orders_table_props);

    // Total current positions chart
    const JSONArray current_positions_chart_data =
        utils::CreateOpenPositionsPieChartData(usd_converted_open_trades_vector);

    Node current_positions_pie_chart =
        ResponsiveContainer({PieChart({Tooltip(),
                                       Legend(),
                                       Pie(
                                           {
                                               Cell({}, props({{"fill", "#4A90E2"}})), // profit
                                               Cell({}, props({{"fill", "#7ED321"}})), // lose
                                           },
                                           props({{"dataKey", "value"},
                                                  {"nameKey", "name"},
                                                  {"data", current_positions_chart_data},
                                                  {"cx", "50%"},
                                                  {"cy", "50%"},
                                                  {"outerRadius", 100.0},
                                                  {"label", true}}))})},
                            props({{"width", "100%"}, {"height", 300.0}}));

    // Top open profit orders table
    std::vector<ReportTradeRecord> top_open_profit_orders_vector =
        utils::CreateTopProfitOrdersVector(open_trades_vector);
    TableBuilder top_open_profit_orders_table_builder("TopOpenProfitOrdersTable");

    top_open_profit_orders_table_builder.SetIdColumn("order");
    top_open_profit_orders_table_builder.SetOrderBy("profit", "DESC");
    top_open_profit_orders_table_builder.EnableAutoSave(false);
    top_open_profit_orders_table_builder.EnableRefreshButton(false);
    top_open_profit_orders_table_builder.EnableBookmarksButton(false);
    top_open_profit_orders_table_builder.EnableExportButton(true);

    top_open_profit_orders_table_builder.AddColumn({"order", "ORDER", 1, search_filter});
    top_open_profit_orders_table_builder.AddColumn({"login", "LOGIN", 2, search_filter});
    top_open_profit_orders_table_builder.AddColumn({"name", "NAME", 3, search_filter});
    top_open_profit_orders_table_builder.AddColumn({"symbol", "SYMBOL", 4, search_filter});
    top_open_profit_orders_table_builder.AddColumn({"group", "GROUP", 5, group_select_filter});
    top_open_profit_orders_table_builder.AddColumn({"type", "TYPE", 6});
    top_open_profit_orders_table_builder.AddColumn({"volume", "VOLUME", 7, search_filter});
    top_open_profit_orders_table_builder.AddColumn({"open_price", "OPEN_PRICE", 8, search_filter});
    top_open_profit_orders_table_builder.AddColumn({"storage", "SWAP", 9, search_filter});
    top_open_profit_orders_table_builder.AddColumn({"profit", "AMOUNT", 10, search_filter});
    top_open_profit_orders_table_builder.AddColumn({"currency", "CURRENCY", 10, search_filter});

    for (const auto& trade : top_open_profit_orders_vector) {
        ReportAccountRecord account_record;
        std::string currency = utils::GetGroupCurrencyByName(groups_vector, account_record.group);

        try {
            server->GetAccountByLogin(trade.login, &account_record);
        } catch (const std::exception& e) {
            std::cerr << "[DailyTradesReportInterface]: " << e.what() << std::endl;
        }

        top_open_profit_orders_table_builder.AddRow(
            {utils::TruncateDouble(trade.order, 0),
             utils::TruncateDouble(trade.login, 1),
             account_record.name,
             trade.symbol,
             account_record.group,
             utils::ConvertCmdToString(static_cast<int>(trade.cmd)),
             utils::TruncateDouble(trade.volume / 100.0, 2),
             utils::TruncateDouble(trade.close_price, 2),
             utils::TruncateDouble(trade.storage, 2),
             utils::TruncateDouble(trade.profit, 2),
             currency});
    }

    const JSONObject top_open_profit_orders_table_props =
        top_open_profit_orders_table_builder.CreateTableProps();
    const Node top_open_profit_orders_table_node = Table({}, top_open_profit_orders_table_props);

    // Top open loss orders table
    std::vector<ReportTradeRecord> top_open_loss_orders_vector =
        utils::CreateTopLossOrdersVector(open_trades_vector);
    TableBuilder top_open_loss_orders_table_builder("TopOpenLossOrdersTable");

    top_open_loss_orders_table_builder.SetIdColumn("order");
    top_open_loss_orders_table_builder.SetOrderBy("profit", "ASC");
    top_open_loss_orders_table_builder.EnableAutoSave(false);
    top_open_loss_orders_table_builder.EnableRefreshButton(false);
    top_open_loss_orders_table_builder.EnableBookmarksButton(false);
    top_open_loss_orders_table_builder.EnableExportButton(true);

    top_open_loss_orders_table_builder.AddColumn({"order", "ORDER", 1, search_filter});
    top_open_loss_orders_table_builder.AddColumn({"login", "LOGIN", 2, search_filter});
    top_open_loss_orders_table_builder.AddColumn({"name", "NAME", 3, search_filter});
    top_open_loss_orders_table_builder.AddColumn({"symbol", "SYMBOL", 4, search_filter});
    top_open_loss_orders_table_builder.AddColumn({"group", "GROUP", 5, group_select_filter});
    top_open_loss_orders_table_builder.AddColumn({"type", "TYPE", 6});
    top_open_loss_orders_table_builder.AddColumn({"volume", "VOLUME", 7, search_filter});
    top_open_loss_orders_table_builder.AddColumn({"open_price", "CLOSE_PRICE", 8, search_filter});
    top_open_loss_orders_table_builder.AddColumn({"storage", "SWAP", 9, search_filter});
    top_open_loss_orders_table_builder.AddColumn({"profit", "AMOUNT", 10, search_filter});
    top_open_loss_orders_table_builder.AddColumn({"currency", "CURRENCY", 10, search_filter});

    for (const auto& trade : top_open_loss_orders_vector) {
        ReportAccountRecord account_record;
        std::string currency = utils::GetGroupCurrencyByName(groups_vector, account_record.group);

        try {
            server->GetAccountByLogin(trade.login, &account_record);
        } catch (const std::exception& e) {
            std::cerr << "[DailyTradesReportInterface]: " << e.what() << std::endl;
        }

        top_open_loss_orders_table_builder.AddRow(
            {utils::TruncateDouble(trade.order, 0),
             utils::TruncateDouble(trade.login, 0),
             account_record.name,
             trade.symbol,
             account_record.group,
             utils::ConvertCmdToString(static_cast<int>(trade.cmd)),
             utils::TruncateDouble(trade.volume / 100.0, 2),
             utils::TruncateDouble(trade.close_price, 2),
             utils::TruncateDouble(trade.storage, 2),
             utils::TruncateDouble(trade.profit, 2),
             currency});
    }

    const JSONObject top_open_loss_orders_table_props =
        top_open_loss_orders_table_builder.CreateTableProps();
    const Node top_open_loss_orders_table_node = Table({}, top_open_loss_orders_table_props);

    // Total report
    const Node report =
        Column({h1({text("Daily Trades Report")}),
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
                top_open_loss_orders_table_node});

    utils::CreateUI(report, response, allocator);
}