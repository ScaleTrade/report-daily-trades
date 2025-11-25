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

    std::cout << "from: : " << from << std::endl;
    std::cout << "to : " << to << std::endl;
    std::cout << "2w from : " << from_two_weeks_ago << std::endl;
    std::cout << "2w to : " << to_two_weeks_ago << std::endl;

    std::vector<TradeRecord> close_trades_vector;

    try {
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

    const Node report = div({
        h1({ text("Daily Trades Report") }),
        h2({ text("Profit and Loss of Clients, USD") }),
        pnl_chart
    });

    utils::CreateUI(report, response, allocator);
}