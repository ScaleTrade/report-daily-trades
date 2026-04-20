#pragma once

#include <string>
#include <unordered_map>

struct ReportGroupRecordSec {
    int sec_index = 0;
    int show = 0;
    int trade = 0;
    int execution = 0;
    double comm_base = 0.0;
    int comm_type = 0;
    int comm_lots = 0;
    double comm_agent = 0.0;
    int comm_agent_type = 0;
    int spread_diff = 0;
    int lot_min = 0;
    int lot_max = 0;
    int lot_step = 0;
    int ie_deviation = 0;
    int confirmation = 0;
    int trade_rights = 0;
    int ie_quick_mode = 0;
    int autocloseout_mode = 0;
    double comm_tax = 0.0;
    int comm_agent_lots = 0;
    int freemargin_mode = 0;
    int reserved[3] = {0, 0, 0};
};

struct ReportGroupRecordMargin {
    std::string symbol;
    int swap_enable = 0;
    double swap_long = 0.0;
    double swap_short = 0.0;
    double margin_divider = 0.0;
    int spread_enable = 0;
    int spread = 0;
    int spread_balance = 0;
    int reserved[7] = {0, 0, 0, 0, 0, 0, 0};
};

struct ReportGroupRecord {
    int grp_index = 0;
    std::string group;
    int enable = 0;
    int timeout = 0;
    int otp_mode = 0;
    std::string company;
    std::string signature;
    std::string support_page;
    std::string smtp_server;
    std::string smtp_login;
    std::string smtp_password;
    std::string support_email;
    std::string templates;
    int copies = 0;
    int reports = 0;
    int default_leverage = 0;
    double default_deposit = 0.0;
    int maxsecurities = 0;
    std::unordered_map<int, ReportGroupRecordSec> secgroups;
    std::unordered_map<std::string, ReportGroupRecordMargin> secmargins;
    int secmargins_total = 0;
    std::string currency;
    double credit = 0.0;
    int margin_call = 0;
    int margin_mode = 1;
    int margin_stopout = 0;
    double interestrate = 0.0;
    int use_swap = 0;
    int news = 0;
    int rights = 0;
    int check_ie_prices = 0;
    int maxpositions = 0;
    int close_reopen = 0;
    int hedge_prohibited = 0;
    int close_fifo = 0;
    int hedge_largeleg = 0;
    std::string securities_hash;
    int margin_type = 0;
    int archive_period = 0;
    int archive_max_balance = 0;
    int stopout_skip_hedged = 0;
    int archive_pending_period = 0;
    unsigned int news_languages[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    unsigned int news_languages_total = 0;
    int reserved[17] = {};
};
