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
        from_two_weeks_ago = utils::CalculateForTwoWeeksAgo(from);
    }
    if (request.HasMember("to") && request["to"].IsNumber()) {
        to = request["to"].GetInt();
        to_two_weeks_ago = utils::CalculateForTwoWeeksAgo(to);
    }

    std::vector<TradeRecord> close_trades_vector;

    try {
        server->GetCloseTradesByGroup(group_mask, from_two_weeks_ago, to_two_weeks_ago, &close_trades_vector);
    } catch (const std::exception& e) {
        std::cerr << "[DailyTradesReportInterface]: " << e.what() << std::endl;
    }

    std::cout << "Close trades vector size: " << close_trades_vector.size() << std::endl;

    // Лямбда подготавливающая значения double для вставки в AST (округление до 2-х знаков)
    auto format_for_AST = [](double value) -> std::string {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << value;
        return oss.str();
    };

    // Test
    struct DataPoint {
        std::string name;
        int line1;
        int line2;
        int line3;
    };

    std::vector<DataPoint> data_points = {{ "Jan", 400, 240, 300 },
    { "2025.11.12", 400, 240, 300 },
    { "2025.11.13", 300, 139, 220 },
    { "2025.11.14", 200, 980, 250 },
    { "2025.11.15", 278, 390, 210 },
    { "2025.11.16", 189, 480, 280 },
    { "2025.11.17", 239, 380, 340 },
    { "2025.11.18", 349, 430, 360 },
    { "2025.11.19", 410, 320, 310 },
    { "2025.11.20", 380, 510, 290 },
    { "2025.11.21", 295, 420, 330 },
    { "2025.11.22", 260, 360, 300 },
    { "2025.11.23", 310, 400, 350 },
    { "2025.11.24", 330, 450, 370 },
    { "2025.11.25", 360, 480, 390 }
    };

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
            {"data", JSONArray{}}
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