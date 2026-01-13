#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "Structures.h"
#include "ast/Ast.hpp"
#include "structures/PluginStructures.h"
#include <rapidjson/document.h>

using namespace ast;

namespace utils {
    void CreateUI(const ast::Node&                    node,
                  rapidjson::Value&                   response,
                  rapidjson::Document::AllocatorType& allocator);

    std::string FormatTimestampToString(const time_t&      timestamp,
                                        const std::string& format = "%Y.%m.%d %H:%M:%S");

    double TruncateDouble(const double& value, const int& digits);

    std::string GetGroupCurrencyByName(const std::vector<GroupRecord>& group_vector,
                                       const std::string&              group_name);

    int CalculateTimestampForTwoWeeksAgo(const int& timestamp);

    std::string FormatDateForChart(const time_t& time);

    JSONArray CreatePnlChartData(const std::vector<UsdConvertedTrade>& trades);

    JSONArray CreateTradesCountChartData(const std::vector<TradeRecord>& trades);

    JSONArray CreateOpenPositionsPieChartData(const std::vector<UsdConvertedTrade>& trades);

    std::vector<TradeRecord> CreateTopProfitOrdersVector(const std::vector<TradeRecord>& trades);

    std::vector<TradeRecord> CreateTopLossOrdersVector(const std::vector<TradeRecord>& trades);
} // namespace utils