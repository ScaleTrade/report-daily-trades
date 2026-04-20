#pragma once

#include <ctime>
#include <string>

struct ReportConSession {
    short open_hour = 0;
    short open_min = 0;
    short close_hour = 0;
    short close_min = 0;
    int open = 0;
    int close = 0;
    short align[7] = {0, 0, 0, 0, 0, 0, 0};
};

struct ReportConSessions {
    ReportConSession quote[3];
    ReportConSession trade[3];
    int quote_overnight = 0;
    int trade_overnight = 0;
    int reserved[2] = {0, 0};
};

struct ReportSymbolRecord {
    std::string symbol;
    std::string description;
    std::string source;
    int feeder = 0;
    std::string currency;
    int sec_index = 0;
    int digits = 0;
    int trade = 0;
    int sym_index = 0;
    int sort_index = 0;
    std::string background_color = "#ffffff";
    int count = 0;
    int count_original = 0;
    ReportConSessions sessions[7];
    int profit_mode = 0;
    int profit_reserved = 0;
    int filter = 0;
    int filter_counter = 1;
    double filter_limit = 0.0;
    int filter_smoothing = 0;
    float filter_reserved = 0.0f;
    int logging = 0;
    int spread = 0;
    int spread_balance = 0;
    int exemode = 0;
    int swap_enable = 0;
    int swap_type = 0;
    double swap_long = 0.0;
    double swap_short = 0.0;
    int swap_rollover3days = 0;
    int swap_daily = 0;
    double contract_size = 0.0;
    double tick_value = 0.0;
    double tick_size = 0.0;
    int stops_level = 0;
    int gtc_pendings = 0;
    int margin_mode = 0;
    double margin_initial = 0.0;
    double margin_maintenance = 0.0;
    double margin_hedged = 0.0;
    double margin_divider = 0.0;
    double point = 0.0;
    double multiply = 0.0;
    double bid_tickvalue = 0.0;
    double ask_tickvalue = 0.0;
    time_t tick_time = 0;
    double bid = 0.0;
    double ask = 0.0;
    int long_only = 0;
    int instant_max_volume = 0;
    int realtime = 1;
    time_t starting = 0;
    time_t expiration = 0;
    std::string quote_currency;
    std::string margin_currency;
    int freeze_level = 0;
    int margin_hedged_strong = 0;
    time_t value_date = 0;
    int quotes_delay = 0;
    int swap_openprice = 0;
    int swap_variation_margin = 0;
    int unused[21] = {};
};
