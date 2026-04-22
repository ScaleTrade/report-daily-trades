#pragma once

#include <ctime>
#include <string>

struct ConvertedTrade {
    time_t close_time;
    double profit;
};

struct PnlDataPoint {
    std::string date;
    int         profit;
    int         loss;
    int         total;
};

struct TradesCountDataPoint {
    std::string date;
    int         profit;
    int         loss;
};

struct OpenPositionsPieDataPoint {
    std::string name;
    double      value = 0.0;
};