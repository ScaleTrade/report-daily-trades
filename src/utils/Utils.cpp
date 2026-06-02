#include "Utils.h"

namespace utils {
    void CreateUI(const ast::Node&                    node,
                  rapidjson::Value&                   response,
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

    std::string FormatTimestampToString(const time_t& timestamp, const std::string& format) {
        std::tm tm{};
        localtime_r(&timestamp, &tm);

        std::ostringstream oss;
        oss << std::put_time(&tm, format.c_str());
        return oss.str();
    }

    double TruncateDouble(const double& value, const int& digits) {
        const double factor = std::pow(10.0, digits);
        return std::trunc(value * factor) / factor;
    }

    std::string GetGroupCurrencyByName(const std::vector<ReportGroupRecord>& group_vector,
                                       const std::string&                    group_name) {
        for (const auto& group : group_vector) {
            if (group.group == group_name) {
                return group.currency;
            }
        }
        return "N/A"; // группа не найдена - валюта не определена
    }

    int CalculateTimestampForTwoWeeksAgo(const int& timestamp) {
        constexpr int two_weeks_interval = 14 * 24 * 60 * 60;
        return timestamp - two_weeks_interval;
    }

    std::string FormatDateForChart(const time_t& time) {
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

    JSONArray CreatePnlChartData(const std::vector<ConvertedTrade>& trades) {
        std::map<std::string, PnlDataPoint> daily_data;

        for (const auto& trade : trades) {
            std::string day = FormatDateForChart(trade.close_time);

            auto& data_point = daily_data[day];
            data_point.date  = day;

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

        std::sort(data_points.begin(),
                  data_points.end(),
                  [](const PnlDataPoint& a, const PnlDataPoint& b) { return a.date < b.date; });

        JSONArray chart_data;
        for (const auto& data_point : data_points) {
            JSONObject point;
            point["day"]         = JSONValue(data_point.date);
            point["profit"]      = JSONValue(static_cast<double>(data_point.profit));
            point["loss"]        = JSONValue(static_cast<double>(data_point.loss));
            point["profit/loss"] = JSONValue(static_cast<double>(data_point.total));

            chart_data.emplace_back(point);
        }

        return chart_data;
    }

    JSONArray CreateTradesCountChartData(const std::vector<ReportTradeRecord>& trades) {
        std::map<std::string, TradesCountDataPoint> daily_data;

        for (const auto& trade : trades) {
            std::string day = FormatDateForChart(trade.close_time);

            auto& data_point = daily_data[day];
            data_point.date  = day;

            if (trade.profit > 0) {
                data_point.profit += 1;
            } else {
                data_point.loss += 1;
            }
        }

        std::vector<TradesCountDataPoint> data_points;
        for (const auto& [date, data_point] : daily_data) {
            data_points.push_back(data_point);
        }

        std::sort(data_points.begin(),
                  data_points.end(),
                  [](const TradesCountDataPoint& a, const TradesCountDataPoint& b) {
                      return a.date < b.date;
                  });

        JSONArray chart_data;
        for (const auto& data_point : data_points) {
            JSONObject point;
            point["day"]    = JSONValue(data_point.date);
            point["profit"] = JSONValue(static_cast<double>(data_point.profit));
            point["loss"]   = JSONValue(static_cast<double>(data_point.loss));

            chart_data.emplace_back(point);
        }

        return chart_data;
    }

    JSONArray CreateOpenPositionsPieChartData(const std::vector<ConvertedTrade>& trades) {
        double total_profit = 0.0;
        double total_loss   = 0.0;

        for (const auto& trade : trades) {
            if (trade.profit >= 0)
                total_profit += trade.profit;
            else
                total_loss += -trade.profit; // убыток как положительное число
        }

        double total = total_profit + total_loss;
        if (total == 0.0)
            return JSONArray{}; // нет открытых позиций с P/L

        JSONArray chart_data;

        auto round2 = [](double value) -> double { return std::round(value * 100.0) / 100.0; };

        if (total_profit > 0) {
            JSONObject profit_point;
            profit_point["name"]  = JSONValue("Profit");
            profit_point["value"] = JSONValue(round2((total_profit / total) * 100.0));
            chart_data.emplace_back(profit_point);
        }

        if (total_loss > 0) {
            JSONObject loss_point;
            loss_point["name"]  = JSONValue("Loss");
            loss_point["value"] = JSONValue(round2((total_loss / total) * 100.0));
            chart_data.emplace_back(loss_point);
        }

        return chart_data;
    }

    std::vector<ReportTradeRecord>
    CreateTopProfitOrdersVector(const std::vector<ReportTradeRecord>& trades) {
        std::vector<ReportTradeRecord> result = trades;
        size_t                         k      = std::min(result.size(), size_t(10));
        std::partial_sort(result.begin(),
                          result.begin() + k,
                          result.end(),
                          [](const ReportTradeRecord& a, const ReportTradeRecord& b) {
                              return a.profit > b.profit;
                          });
        result.resize(k);
        return result;
    }

    std::vector<ReportTradeRecord>
    CreateTopLossOrdersVector(const std::vector<ReportTradeRecord>& trades) {
        std::vector<ReportTradeRecord> result = trades;
        size_t                         k      = std::min(result.size(), size_t(10));
        std::partial_sort(result.begin(),
                          result.begin() + k,
                          result.end(),
                          [](const ReportTradeRecord& a, const ReportTradeRecord& b) {
                              return a.profit < b.profit;
                          });
        result.resize(k);
        return result;
    }

    std::string ConvertCmdToString(const int cmd) {
        switch (cmd) {
            case -1:
                return "Nothing";
            case 0:
                return "Buy";
            case 1:
                return "Sell";
            case 2:
                return "Buy Limit";
            case 3:
                return "Sell Limit";
            case 4:
                return "Buy Stop";
            case 5:
                return "Sell Stop";
            case 6:
                return "Deposit";
            case 7:
                return "Credit In";
            case 8:
                return "Withdrawal";
            case 9:
                return "Credit Out";
            case 10:
                return "Buy Stop Limit";
            case 11:
                return "Sell Stop Limit";
            default:
                return "Unknown";
        }
    }

    std::string Trim(const std::string& str) {
        const auto begin = str.find_first_not_of(" \t");
        if (begin == std::string::npos)
            return "";
        const auto end = str.find_last_not_of(" \t");
        return str.substr(begin, end - begin + 1);
    }

    std::set<std::string> SplitToSet(const std::string& str) {
        std::set<std::string> out;
        std::stringstream     ss(str);
        std::string           item;
        while (std::getline(ss, item, ',')) {
            const std::string trimmed = Trim(item);
            if (!trimmed.empty())
                out.insert(trimmed);
        }
        return out;
    }
} // namespace utils