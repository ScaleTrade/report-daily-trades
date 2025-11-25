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
#include <iostream>
#include "ast/Ast.hpp"
#include <rapidjson/document.h>
#include "Structures.hpp"
#include "PluginStructures.hpp"

using namespace ast;

namespace utils {
    void CreateUI(const ast::Node& node,
              rapidjson::Value& response,
              rapidjson::Document::AllocatorType& allocator);

    int CalculateTimestampForTwoWeeksAgo(const int timestamp);

    std::string FormatDateForChart(time_t time);

    JSONArray CreatePnlChartData(const std::vector<TradeRecord>& trades);

    JSONArray CreateTradesCountChartData(const std::vector<TradeRecord>& trades);

    JSONArray CreateOpenPositionsPieChartData(const std::vector<TradeRecord>& trades);

    std::vector<TradeRecord> CreateTopProfitOrdersVector(const std::vector<TradeRecord>& trades);
    std::vector<TradeRecord> CreateTopLossOrdersVector(const std::vector<TradeRecord>& trades);
}