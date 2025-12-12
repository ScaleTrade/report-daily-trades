#pragma once

#include <cmath>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cstddef>
#include <iostream>
#include "ast/Ast.hpp"
#include <rapidjson/document.h>
#include "Structures.hpp"
#include "structures/PluginStructures.hpp"

using namespace ast;

namespace utils {
    void CreateUI(const ast::Node& node,
              rapidjson::Value& response,
              rapidjson::Document::AllocatorType& allocator);

    std::string FormatTimestampToString(const time_t& timestamp);

    double TruncateDouble(const double& value, const int& digits);

    std::string GetGroupCurrencyByName(const std::vector<GroupRecord>& group_vector, const std::string& group_name);

    int CalculateTimestampForTwoWeeksAgo(const int& timestamp);

    std::string FormatDateForChart(const time_t& time);

    JSONArray CreatePnlChartData(const std::vector<UsdConvertedTrade>& trades);

    JSONArray CreateTradesCountChartData(const std::vector<TradeRecord>& trades);

    JSONArray CreateOpenPositionsPieChartData(const std::vector<UsdConvertedTrade>& trades);

    std::vector<TradeRecord> CreateTopProfitOrdersVector(const std::vector<TradeRecord>& trades);

    std::vector<TradeRecord> CreateTopLossOrdersVector(const std::vector<TradeRecord>& trades);
}