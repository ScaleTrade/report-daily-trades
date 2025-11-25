#pragma once

#include <string>

struct PnlDataPoint {
    std::string date;
    int profit;
    int loss;
    int total;
};