#pragma once

#include <ctime>
#include <string>

struct ReportMarginLevel {
    int login = 0;
    std::string group;
    int leverage = 0;
    double balance = 0.0;
    double credit = 0.0;
    double equity = 0.0;
    double profit = 0.0;
    double storage = 0.0;
    double commission = 0.0;
    double margin = 0.0;
    double margin_free = 0.0;
    double margin_level = 0.0;
    int margin_type = 0;
    int level_type = 0;
};

struct ReportEquityRecord {
    int login = 0;
    time_t create_time = 0;
    std::string group;
    double balance = 0.0;
    double prevbalance = 0.0;
    double credit = 0.0;
    double equity = 0.0;
    double profit = 0.0;
    double storage = 0.0;
    double commission = 0.0;
    double margin = 0.0;
    double margin_free = 0.0;
    double margin_level = 0.0;
    std::string currency;
};

struct ReportAccountRecord {
    int login = 0;
    std::string group;
    std::string password;
    int enable = 1;
    int enable_change_password = 0;
    int enable_read_only = 0;
    int enable_otp = 0;
    int enable_reserved[2] = {0, 0};
    std::string password_investor;
    std::string avatar;
    std::string name;
    std::string country;
    std::string city;
    std::string state;
    std::string zipcode;
    std::string address;
    std::string lead_source;
    std::string phone;
    std::string email;
    std::string comment;
    std::string id;
    std::string status;
    time_t regdate = 0;
    time_t lastdate = 0;
    int leverage = 0;
    int agent_account = 0;
    time_t timestamp = 0;
    int last_ip = 0;
    double balance = 0.0;
    double prevmonthbalance = 0.0;
    double prevbalance = 0.0;
    double credit = 0.0;
    double interestrate = 0.0;
    double taxes = 0.0;
    double prevmonthequity = 0.0;
    double prevequity = 0.0;
    double reserved2[2] = {0.0, 0.0};
    std::string otp_secret;
    std::string secure_reserved;
    int send_reports = 0;
    int mqid = 0;
    std::string user_color = "#ffffff";
    std::string unused;
    std::string api_data;
    ReportMarginLevel margin;
};
