#pragma once

#include <ctime>

struct ReportCandleRecord {
    double open = 0.0;
    double high = 0.0;
    double low = 0.0;
    double close = 0.0;
    time_t time = 0;
    double volume = 0.0;
};
