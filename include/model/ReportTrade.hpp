#pragma once

#include <array>
#include <ctime>
#include <string>

#include "ReportTradeEnums.hpp"

struct ReportTradeRecord {
    int order = 0;
    int login = 0;
    std::string symbol;
    int digits = 2;
    ReportTradeCommand cmd = ReportTradeCommand::Nothing;
    int volume = 0;
    time_t open_time = 0;
    ReportTradeState state = ReportTradeState::NoChange;
    double open_price = 0.0;
    double sl = 0.0;
    double tp = 0.0;
    double margin_initial = 0.0;
    time_t close_time = 0;
    int gw_volume = 0;
    time_t expiration = 0;
    ReportTradeReason reason = ReportTradeReason::Client;
    std::array<double, 2> conv_rates{};
    double commission = 0.0;
    double prev_commission = 0.0;
    double commission_agent = 0.0;
    double storage = 0.0;
    double prev_storage = 0.0;
    double profit = 0.0;
    double prev_profit = 0.0;
    double close_price = 0.0;
    double taxes = 0.0;
    int magic = 0;
    std::string comment;
    std::string gw_order;
    std::string gw_source;
    std::string gw_uuid;
    ReportTradeActivation activation = ReportTradeActivation::None;
    double gw_open_price = 0.0;
    double gw_close_price = 0.0;
    double margin_rate = 0.0;
    std::string api_data;
    time_t last_swap_time = 0;
    time_t update_time = 0;
};
