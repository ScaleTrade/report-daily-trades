#include "Utils.hpp"

namespace utils {
    void CreateUI(const ast::Node& node,
                  rapidjson::Value& response,
                  rapidjson::Document::AllocatorType& allocator) {
        // Content
        Value node_object(kObjectType);
        to_json(node, node_object, allocator);

        Value content_array(kArrayType);
        content_array.PushBack(node_object, allocator);

        // Header
        Value header_array(kArrayType);

        {
            Value space_object(kObjectType);
            space_object.AddMember("type", "Space", allocator);

            Value children(kArrayType);

            Value text_object(kObjectType);
            text_object.AddMember("type", "#text", allocator);

            Value props(kObjectType);
            props.AddMember("value", "Daily Trades report", allocator);

            text_object.AddMember("props", props, allocator);
            children.PushBack(text_object, allocator);

            space_object.AddMember("children", children, allocator);

            header_array.PushBack(space_object, allocator);
        }

        // Footer
        Value footer_array(kArrayType);

        {
            Value space_object(kObjectType);
            space_object.AddMember("type", "Space", allocator);

            Value props_space(kObjectType);
            props_space.AddMember("justifyContent", "space-between", allocator);
            space_object.AddMember("props", props_space, allocator);

            Value children(kArrayType);

            Value btn_object(kObjectType);
            btn_object.AddMember("type", "Button", allocator);

            Value btn_props_object(kObjectType);
            btn_props_object.AddMember("className", "form_action_button", allocator);
            btn_props_object.AddMember("borderType", "danger", allocator);
            btn_props_object.AddMember("buttonType", "outlined", allocator);

            btn_props_object.AddMember("onClick", "{\"action\":\"CloseModal\"}", allocator);

            btn_object.AddMember("props", btn_props_object, allocator);

            Value btn_children(kArrayType);

            Value text_object(kObjectType);
            text_object.AddMember("type", "#text", allocator);

            Value text_props_object(kObjectType);
            text_props_object.AddMember("value", "Close", allocator);

            text_object.AddMember("props", text_props_object, allocator);
            btn_children.PushBack(text_object, allocator);

            btn_object.AddMember("children", btn_children, allocator);

            children.PushBack(btn_object, allocator);

            space_object.AddMember("children", children, allocator);

            footer_array.PushBack(space_object, allocator);
        }


        // Modal
        Value model_object(kObjectType);
        model_object.AddMember("size", "xxxl", allocator);
        model_object.AddMember("headerContent", header_array, allocator);
        model_object.AddMember("footerContent", footer_array, allocator);
        model_object.AddMember("content", content_array, allocator);


        // UI
        Value ui_object(kObjectType);
        ui_object.AddMember("modal", model_object, allocator);

        response.SetObject();
        response.AddMember("ui", ui_object, allocator);
    }

    int CalculateTimestampForTwoWeeksAgo(const int timestamp) {
        constexpr int two_weeks_interval = 14 * 24 * 60 * 60;
        return timestamp - two_weeks_interval;
    }

    std::string FormatDateForChart(time_t time) {
        std::tm tm{};
        #ifdef _WIN32
                localtime_s(&tm, &t);
        #else
                localtime_r(&time, &tm);
        #endif
                std::ostringstream oss;
                oss << std::put_time(&tm, "%Y.%m.%d");
                return oss.str();
            }

    JSONArray CreatePnlChartData(const std::vector<TradeRecord>& trades) {
        std::cout << "Trades length: " << trades.size() << std::endl;

        // Mock data
        std::vector<PnlDataPoint> data_points_mock = {
            { "2025.11.12", 500, -100, 400 },
            { "2025.11.13", 350, -50, 300 },
            { "2025.11.14", 250, -50, 200 },
            { "2025.11.15", 320, -42, 278 },
            { "2025.11.16", 220, -31, 189 },
            { "2025.11.17", 280, -41, 239 },
            { "2025.11.18", 420, -71, 349 },
            { "2025.11.19", 500, -90, 410 },
            { "2025.11.20", 450, -70, 380 },
            { "2025.11.21", 350, -55, 295 },
            { "2025.11.22", 300, -40, 260 },
            { "2025.11.23", 370, -60, 310 },
            { "2025.11.24", 400, -70, 330 },
            { "2025.11.25", 450, -90, 360 }
        };

        std::map<std::string, PnlDataPoint> daily_data;

        for (const auto& trade : trades) {
            std::string day = FormatDateForChart(trade.close_time);

            auto& data_point = daily_data[day];
            data_point.date = day;

            if (trade.profit > 0) {
                data_point.profit += trade.profit;
            } else {
                data_point.loss += trade.profit;
            }

            data_point.total += trade.profit;
        }

        std::vector<PnlDataPoint> data_points;
        for (const auto& [date, data_point] : daily_data) {
            data_points.push_back(data_point);
        }

        std::cout << "Data points length: " << data_points.size() << std::endl;

        std::sort(data_points.begin(), data_points.end(), [](const PnlDataPoint& a,
                                                                           const PnlDataPoint& b) {
            return a.date < b.date;
        });

        JSONArray chart_data;
        for (const auto& data_point : data_points) {
            JSONObject point;
            point["day"]   = JSONValue(data_point.date);
            point["profit"] = JSONValue(static_cast<double>(data_point.profit));
            point["loss"] = JSONValue(static_cast<double>(data_point.loss));
            point["profit/loss"] = JSONValue(static_cast<double>(data_point.total));

            chart_data.emplace_back(point);
        }

        return chart_data;
    }
}