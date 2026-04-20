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

#include "ReportServerInterface.h"
#include "ast/Ast.hpp"
#include "structures/ReportStructures.h"
#include <rapidjson/document.h>

using namespace ast;

namespace utils {
    void CreateUI(const ast::Node&                    node,
                  rapidjson::Value&                   response,
                  rapidjson::Document::AllocatorType& allocator);

    std::string FormatTimestampToString(const time_t&      timestamp,
                                        const std::string& format = "%Y.%m.%d %H:%M:%S");

    double TruncateDouble(const double& value, const int& digits);

    std::string GetGroupCurrencyByName(const std::vector<ReportGroupRecord>& group_vector,
                                       const std::string&                    group_name);

    int CalculateTimestampForTwoWeeksAgo(const int& timestamp);

    std::string FormatDateForChart(const time_t& time);

    JSONArray CreatePnlChartData(const std::vector<UsdConvertedTrade>& trades);

    JSONArray CreateTradesCountChartData(const std::vector<ReportTradeRecord>& trades);

    JSONArray CreateOpenPositionsPieChartData(const std::vector<UsdConvertedTrade>& trades);

    std::vector<ReportTradeRecord>
    CreateTopProfitOrdersVector(const std::vector<ReportTradeRecord>& trades);

    std::vector<ReportTradeRecord>
    CreateTopLossOrdersVector(const std::vector<ReportTradeRecord>& trades);

    std::string ConvertCmdToString(const int cmd);
} // namespace utils