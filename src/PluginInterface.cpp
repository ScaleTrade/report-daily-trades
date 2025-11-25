#include "PluginInterface.hpp"

#include <iomanip>

using namespace ast;

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
        server->GetCloseTradesByGroup(group_mask, from_two_weeks_ago, to_two_weeks_ago, &close_trades_vector);
    } catch (const std::exception& e) {
        std::cerr << "[DailyTradesReportInterface]: " << e.what() << std::endl;
    }

    // Лямбда подготавливающая значения double для вставки в AST (округление до 2-х знаков)
    auto format_for_AST = [](double value) -> std::string {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << value;
        return oss.str();
    };

    // Test
    struct DataPoint {
        std::string date;
        int line1;
    };

    std::vector<DataPoint> data_points = {
    { "2025.11.12", 400},
    { "2025.11.13", 300},
    { "2025.11.14", 200},
    { "2025.11.15", 278},
    { "2025.11.16", 189},
    { "2025.11.17", 239},
    { "2025.11.18", 349},
    { "2025.11.19", 410},
    { "2025.11.20", 380},
    { "2025.11.21", 295},
    { "2025.11.22", 260},
    { "2025.11.23", 310},
    { "2025.11.24", 330},
    { "2025.11.25", 360}
    };

    JSONArray chart_data;
    for (const auto& data_point : data_points) {
        JSONObject point;
        point["day"]   = JSONValue(data_point.date);
        point["line1"] = JSONValue(static_cast<double>(data_point.line1));
        // point["line2"] = JSONValue(data_point.line2);
        // point["line3"] = JSONValue(data_point.line3);

        chart_data.emplace_back(point);
    }

    Node chart = ResponsiveContainer({
        LineChart({
            XAxis({}, props({{"dataKey", "day"}})),
            YAxis(),
            Tooltip(),
            Legend(),

            Line({}, props({
                {"type", "monotone"},
                {"dataKey", "line1"},
                {"stroke", "#8884d8"}
            })),

            Line({}, props({
                {"type", "monotone"},
                {"dataKey", "line2"},
                {"stroke", "#82ca9d"}
            })),

            Line({}, props({
                {"type", "monotone"},
                {"dataKey", "line3"},
                {"stroke", "#ff7300"}
            }))
        }, props({
            {"data", chart_data}
        }))
    }, props({
        {"width", "100%"},
        {"height", 300.0}
    }));

    const Node report = div({
        h1({ text("Daily Trades Report") }),
        chart
    });

    utils::CreateUI(report, response, allocator);
}