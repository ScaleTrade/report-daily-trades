#pragma once

#include <vector>
#include <string>
#include <map>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "ast/Ast.hpp"
#include "Structures.hpp"

namespace utils {
    void CreateUI(const ast::Node& node,
              rapidjson::Value& response,
              rapidjson::Document::AllocatorType& allocator);

    int CalculateTimestampForTwoWeeksAgo(const int timestamp);

    std::string FormatDateForeChart(time_t time);
}