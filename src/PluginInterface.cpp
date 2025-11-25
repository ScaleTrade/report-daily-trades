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
    int to_two_weeks_ago;
    if (request.HasMember("group") && request["group"].IsString()) {
        group_mask = request["group"].GetString();
    }
    if (request.HasMember("from") && request["from"].IsNumber()) {
        from = request["from"].GetInt();
        from_two_weeks_ago = utils::CalculateTimestampForTwoWeeksAgo(from);
    }
    if (request.HasMember("to") && request["to"].IsNumber()) {
        to = request["to"].GetInt();
        to_two_weeks_ago = utils::CalculateTimestampForTwoWeeksAgo(to);
    }

    std::vector<TradeRecord> close_trades_vector;

    try {
        // 1735689600, 1764075600
        server->GetCloseTradesByGroup(group_mask, 1735689600, 1764075600, &close_trades_vector);
    } catch (const std::exception& e) {
        std::cerr << "[DailyTradesReportInterface]: " << e.what() << std::endl;
    }

    std::cout << "Close trades vector size: : " << close_trades_vector.size() << std::endl;

    // Лямбда подготавливающая значения double для вставки в AST (округление до 2-х знаков)
    auto format_for_AST = [](double value) -> std::string {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << value;
        return oss.str();
    };

    // Profit / Lose chart
    const JSONArray pnl_chart_data = utils::CreatePnlChartData(close_trades_vector);

    Node pnl_chart = ResponsiveContainer({
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

    Node trades_count_chart = ResponsiveContainer({
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

    // Top profit orders table
    std::vector<TradeRecord> top_profit_orders_vector = utils::CreateTopProfitOrdersVector(close_trades_vector);

    auto create_top_loss_orders_table = [&](const std::vector<TradeRecord>& trades) -> Node {
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
                td({div({text(std::to_string(trade.close_price))})}),
                td({div({text("Swap")})}),
                td({div({text(std::to_string(trade.profit))})}),
            }));
        }

        return table({
            thead(thead_rows),
            tbody(tbody_rows),
            tfoot(tfoot_rows),
        }, props({{"className", "table"}}));
    };

    // Top loss orders table
    std::vector<TradeRecord> top_loss_orders_vector = utils::CreateTopLossOrdersVector(close_trades_vector);

    auto create_top_profit_orders_table = [&](const std::vector<TradeRecord>& trades) -> Node {
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
                td({div({text(std::to_string(trade.close_price))})}),
                td({div({text("Swap")})}),
                td({div({text(std::to_string(trade.profit))})}),
            }));
        }

        return table({
            thead(thead_rows),
            tbody(tbody_rows),
            tfoot(tfoot_rows),
        }, props({{"className", "table"}}));
    };

    const Node report = div({
        h1({text("Daily Trades Report")}),
        h2({text("Profit and Loss of Clients, USD")}),
        pnl_chart,
        h2({text("Client Trades Count")}),
        trades_count_chart,
        h2({text("Top Profit Orders")}),
        create_top_profit_orders_table(top_profit_orders_vector),
        h2({text("Top Loss Orders")}),
        create_top_loss_orders_table(top_loss_orders_vector),
    });

    utils::CreateUI(report, response, allocator);
}