#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <string>
#include <iostream>
#include <array>
#include <unordered_map>
#include <vector>

#include <rapidjson/document.h>

using namespace rapidjson;

inline int PLUGIN_SERVER_API = 104;

enum ReturnCodes {
    RET_OK = 0,
    RET_OK_NONE,
    RET_ERROR,
    RET_INVALID_DATA,
    RET_TECH_PROBLEM,
    RET_OLD_VERSION,
    RET_NO_CONNECT,
    RET_NOT_ENOUGH_RIGHTS,
    RET_TOO_FREQUENT,
    RET_MALFUNCTION,
    RET_GENERATE_KEY,
    RET_SECURITY_SESSION,
    RET_INVALID_PASSWORD,
    RET_ERR_PARAMS,
    RET_ERR_DATA,
    RET_ERR_DISK,
    RET_ERR_MEM,
    RET_ERR_NETWORK,
    RET_ERR_PERMISSIONS,
    RET_ERR_TIMEOUT,
    RET_ERR_CONNECTION,
    RET_ERR_NOSERVICE,
    RET_ERR_FREQUENT,
    RET_ERR_NOTFOUND,
    RET_ERR_SHUTDOWN,
    RET_ERR_CANCEL,
    RET_ERR_DUPLICATE,
    RET_OK_CHANGE,

    RET_GROUP_NOT_FOUND = 28,
    RET_GROUP_USE_BY_USER,

    RET_SEC_USE_BY_SYMBOL = 32,
    RET_SYMBOL_USE_BY_TRADE,
    RET_SYMBOL_NOT_FOUND,

    RET_USER_NOT_FOUND = 48,
    RET_USR_LAST_ADMIN,
    RET_USR_LOGIN_EXHAUSTED,
    RET_USR_LOGIN_PROHIBITED,
    RET_USR_LOGIN_EXIST,
    RET_USR_SUICIDE,
    RET_USR_INVALID_PASSWORD,
    RET_USR_LIMIT_REACHED,
    RET_USR_HAS_TRADES,
    RET_USR_DIFFERENT_SERVERS,
    RET_USR_DIFFERENT_CURRENCY,
    RET_USR_IMPORT_BALANCE,
    RET_USR_IMPORT_GROUP,
    RET_USR_ACCOUNT_EXIST,

    RET_ACCOUNT_DISABLED = 64,
    RET_BAD_ACCOUNT_INFO,
    RET_PUBLIC_KEY_MISSING,

    RET_TRADE_TIMEOUT = 128,
    RET_TRADE_BAD_PRICES,
    RET_TRADE_BAD_STOPS,
    RET_TRADE_BAD_VOLUME,
    RET_TRADE_MARKET_CLOSED,
    RET_TRADE_DISABLE,
    RET_TRADE_NO_MONEY,
    RET_TRADE_PRICE_CHANGED,
    RET_TRADE_OFFQUOTES,
    RET_TRADE_BROKER_BUSY,
    RET_TRADE_REQUOTE,
    RET_TRADE_ORDER_LOCKED,
    RET_TRADE_LONG_ONLY,
    RET_TRADE_TOO_MANY_REQ,

    RET_TRADE_ACCEPTED = 145,
    RET_TRADE_PROCESS,
    RET_TRADE_USER_CANCEL,

    RET_TRADE_MODIFY_DENIED = 150,
    RET_TRADE_CONTEXT_BUSY,
    RET_TRADE_EXPIRATION_DENIED,
    RET_TRADE_TOO_MANY_ORDERS,
    RET_TRADE_HEDGE_PROHIBITED,
    RET_TRADE_PROHIBITED_BY_FIFO,
    RET_DUPLICATE_RECORD,
    RET_TRADE_SEC_NOT_FOUND,
    RET_TRADE_INVALID_SL_TP,
    RET_TRADE_BAD_CLOSE_TIME,
    RET_TRADE_READ_ONLY = 160,
    RET_TRADE_INCORRECT_STATE,
    RET_TRADE_INCORRECT_CMD,
    RET_TRADE_INCORRECT_ORDER,

    RET_CFG_LAST_ADMIN = 256,
    RET_CFG_LAST_ADMIN_GROUP,
    RET_CFG_NOT_EMPTY,
    RET_CFG_INVALID_RANGE,
    RET_CFG_NOT_MANAGER_LOGIN,
    RET_CFG_BUILTIN,
    RET_CFG_DUPLICATE,
    RET_CFG_LIMIT_REACHED,
    RET_CFG_NO_ACCESS_TO_MAIN,
    RET_CFG_DEALER_ID_EXIST,
    RET_CFG_BIND_ADDR_EXIST,
    RET_CFG_WORKING_TRADE,
    RET_CFG_GATEWAY_NAME_EXIST,
    RET_CFG_SWITCH_TO_BACKUP,
    RET_CFG_NO_BACKUP_MODULE,
    RET_CFG_NO_TRADE_MODULE,
    RET_CFG_NO_HISTORY_MODULE,
    RET_CFG_ANOTHER_SWITCH,
    RET_CFG_NO_LICENSE_FILE,
    RET_CFG_GATEWAY_LOGIN_EXIST,
    RET_LICENSE_NOT_ACTIVE,

    RET_BAD_REQUEST = 400,
    RET_UNAUTHORIZED,
    RET_FIELD_REQUIRED,
    RET_FORBIDDEN,
    RET_NOT_FOUND,
    RET_METHOD_NOT_ALLOWED,
    RET_INVALID_METHOD,
    RET_TIMEOUT
};

enum EventType {
    EV_TYPE_BALANCE = 0,
    EV_TYPE_TRADE,
    EV_TYPE_SYMBOL,
    EV_TYPE_GROUP,
    EV_TYPE_MARKET,
    EV_TYPE_ACCOUNT,
    EV_TYPE_SETTINGS,
    EV_TYPE_NOTIFY
};

enum EventRecordType {
    EV_RECORD_ADD = 0,
    EV_RECORD_UPDATE,
    EV_RECORD_DELETE,
    EV_RECORD_RESTORE,
    EV_RECORD_ARCHIVE,
    EV_RECORD_ACTIVATE_TRADE,
    EV_RECORD_CLOSE_TRADE
};

enum {
    SESSION_USER = 0,
    SESSION_MANAGER,
    SESSION_DEALER,
    SESSION_ADMIN,
    SESSION_SYSTEM,
    SESSION_FIX
};

enum ConnectionType {
    CONNECTION_TYPE_CLIENT_DESKTOP = 0,
    CONNECTION_TYPE_CLIENT_WINPHONE = 1,
    CONNECTION_TYPE_CLIENT_IPHONE = 2,
    CONNECTION_TYPE_CLIENT_ANDROID = 3,
    CONNECTION_TYPE_CLIENT_BLACKBERRY = 4,
    CONNECTION_TYPE_CLIENT_WEB = 5,
    CONNECTION_TYPE_REST = 6,
    CONNECTION_TYPE_TCP = 7,
    CONNECTION_TYPE_FIX = 8
};

enum DbStateType {
    DB_NO_CHANGE = 0,
    DB_NEED_INSERT,
    DB_NEED_UPDATE,
    DB_NEED_DELETE
};

struct TickInfo {
    std::string symbol;
    time_t ctm;
    double volume;
    double bid;
    double ask;
};

inline std::string FormatError(const int retcode) {
    std::cout << "CODE ERROR:" << retcode << std::endl;

    struct ErrorDesc {
        int code;
        std::string desc;
    };
    static ErrorDesc errors[] =
    {
        {RET_OK, "OK"},
        {RET_OK_NONE, "OK/None"},
        {RET_ERROR, "Common error"},
        {RET_ERR_PARAMS, "Invalid parameters"},
        {RET_INVALID_DATA, "Invalid parameters"},
        {RET_ERR_DATA, "Invalid data"},
        {RET_ERR_DISK, "Disk error"},
        {RET_ERR_MEM, "Memory error"},
        {RET_ERR_NETWORK, "Network error"},
        {RET_ERR_PERMISSIONS, "Not enough permissions"},
        {RET_ERR_TIMEOUT, "Operation timeout"},
        {RET_ERR_CONNECTION, "No connection"},
        {RET_ERR_NOSERVICE, "Service is not available"},
        {RET_ERR_FREQUENT, "Too frequent requests"},
        {RET_ERR_NOTFOUND, "Not found"},
        {RET_ERR_SHUTDOWN, "Server shutdown in progress"},
        {RET_ERR_CANCEL, "Operation was canceled"},
        {RET_ERR_DUPLICATE, "Duplicate attempt"},

        {RET_CFG_LAST_ADMIN, "Last admin config cannot be deleted"},
        {RET_CFG_LAST_ADMIN_GROUP, "Last admin group cannot be deleted"},
        {RET_CFG_NOT_EMPTY, "Accounts or trades in group/symbol"},
        {RET_CFG_INVALID_RANGE, "Invalid account or trade ranges"},
        {RET_CFG_NOT_MANAGER_LOGIN, "Account doesn't belong to manager group"},
        {RET_CFG_BUILTIN, "Built-in protected config"},
        {RET_CFG_DUPLICATE, "Configuration duplicate"},
        {RET_CFG_LIMIT_REACHED, "Configuration limit reached"},
        {RET_CFG_NO_ACCESS_TO_MAIN, "Invalid network configuration"},
        {RET_CFG_DEALER_ID_EXIST, "Dealer with same ID already exists"},
        {RET_CFG_BIND_ADDR_EXIST, "Binding address already exists"},
        {RET_CFG_WORKING_TRADE, "Attempt to delete working trade server"},
        {RET_CFG_GATEWAY_NAME_EXIST, "Gateway with same name already exists"},
        {RET_CFG_SWITCH_TO_BACKUP, "Server must be switched to backup mode"},
        {RET_CFG_NO_BACKUP_MODULE, "Backup server module is absent"},
        {RET_CFG_NO_TRADE_MODULE, "Trade server module is absent"},
        {RET_CFG_NO_HISTORY_MODULE, "History server module is absent"},
        {RET_CFG_ANOTHER_SWITCH, "Another switching process in progress"},
        {RET_CFG_NO_LICENSE_FILE, "License file is absent"},
        {RET_CFG_GATEWAY_LOGIN_EXIST, "Gateway with same login already exist"},

        {RET_USR_LAST_ADMIN, "Last admin account cannot be deleted"},
        {RET_USR_LOGIN_EXHAUSTED, "Login range exhausted"},
        {RET_USR_LOGIN_PROHIBITED, "Login reserved at another server"},
        {RET_USR_LOGIN_EXIST, "Account already exists"},
        {RET_USR_SUICIDE, "Attempt of self-deletion"},
        {RET_USR_INVALID_PASSWORD, "Invalid account password"},
        {RET_USR_LIMIT_REACHED, "User limit reached"},
        {RET_USR_HAS_TRADES, "Account has open trades"},
        {RET_USR_DIFFERENT_SERVERS, "Attempt to move account to different server"},
        {RET_USR_DIFFERENT_CURRENCY, "Attempt to move account to group with different currency"},
        {RET_USR_IMPORT_BALANCE, "Account balance import error"},
        {RET_USR_IMPORT_GROUP, "Imported account has invalid group"},
        {RET_USR_ACCOUNT_EXIST, "Account already exist"},

        {RET_TRADE_BAD_PRICES, "Order price its not correct"},

        {RET_TRADE_BAD_STOPS, "wrong stops level"},
        {RET_TRADE_BAD_VOLUME, "wrong lot size"},
        {RET_TRADE_MARKET_CLOSED, "market closed"},
        {RET_TRADE_DISABLE, "Trade disabled"},
        {RET_TRADE_NO_MONEY, "No enough money for order execution"},
        {RET_TRADE_PRICE_CHANGED, "Price changed"},
        {RET_TRADE_OFFQUOTES, "No quotes"},
        {RET_TRADE_BROKER_BUSY, "Broker is busy"},
        {RET_TRADE_REQUOTE, "Requote"},
        {RET_TRADE_ORDER_LOCKED, "Order is proceed by dealer and cannot be changed"},
        {RET_TRADE_LONG_ONLY, "Allowed only BUY orders"},
        {RET_TRADE_TOO_MANY_REQ, "Too many requests from one client"},

        {RET_TRADE_ACCEPTED, "Trade request accepted by server and placed in request queue"},
        {RET_TRADE_PROCESS, "Trade request accepted by dealerd"},
        {RET_TRADE_USER_CANCEL, "Trade request canceled by client"},

        {RET_TRADE_MODIFY_DENIED, "Trade modification denied"},
        {RET_TRADE_CONTEXT_BUSY, "Trade context is busy (used in client terminal)"},
        {RET_TRADE_EXPIRATION_DENIED, "Using expiration date denied"},
        {RET_TRADE_TOO_MANY_ORDERS, "Too many orders"},
        {RET_TRADE_HEDGE_PROHIBITED, "Hedge is prohibited"},
        {RET_TRADE_PROHIBITED_BY_FIFO, "Prohibited by fifo rule"},
        {RET_DUPLICATE_RECORD, "Попытка создание уже существующего трейда"},
        {RET_TRADE_SEC_NOT_FOUND, "Не нашел активную символ-группу (sec) в группе пользователя"},
        {RET_TRADE_INVALID_SL_TP, "Invalid SL or TP"},

        {RET_CFG_LAST_ADMIN, "Last admin config cannot be deleted"},
        {RET_CFG_LAST_ADMIN_GROUP, "Last admin group cannot be deleted"},
        {RET_CFG_NOT_EMPTY, "Accounts or trades in group/symbol"},
        {RET_CFG_INVALID_RANGE, "Invalid account or trade ranges"},
        {RET_CFG_NOT_MANAGER_LOGIN, "Account doesn't belong to manager group"},
        {RET_CFG_BUILTIN, "Built-in protected config"},
        {RET_CFG_DUPLICATE, "Configuration duplicate"},
        {RET_CFG_LIMIT_REACHED, "Configuration limit reached"},
        {RET_CFG_NO_ACCESS_TO_MAIN, "Invalid network configuration"},
        {RET_CFG_DEALER_ID_EXIST, "Dealer with same ID already exists"},
        {RET_CFG_BIND_ADDR_EXIST, "Binding address already exists"},
        {RET_CFG_WORKING_TRADE, "Attempt to delete working trade server"},
        {RET_CFG_GATEWAY_NAME_EXIST, "Gateway with same name already exists"},
        {RET_CFG_SWITCH_TO_BACKUP, "Server must be switched to backup mode"},
        {RET_CFG_NO_BACKUP_MODULE, "Backup server module is absent"},
        {RET_CFG_NO_TRADE_MODULE, "Trade server module is absent"},
        {RET_CFG_NO_HISTORY_MODULE, "History server module is absent"},
        {RET_CFG_ANOTHER_SWITCH, "Another switching process in progress"},
        {RET_CFG_NO_LICENSE_FILE, "License file is absent"},
        {RET_CFG_GATEWAY_LOGIN_EXIST, "Gateway with same login already exist"},
        {RET_LICENSE_NOT_ACTIVE, "Your license has expired! Please renew it to continue using the trading platform."},

    };
    for (auto& error: errors)
        if (error.code == retcode) return (error.desc);
    return "Unknown error";
}

enum {
    OP_NOTHING = -1,
    OP_BUY = 0,
    OP_SELL,
    OP_BUY_LIMIT,
    OP_SELL_LIMIT,
    OP_BUY_STOP,
    OP_SELL_STOP,
    OP_BALANCE_IN = 6,
    OP_CREDIT_IN,
    OP_BALANCE_OUT,
    OP_CREDIT_OUT,
    OP_BUY_STOP_LIMIT = 10,
    OP_SELL_STOP_LIMIT
};

enum {
    TS_NO_CHANGE = -1,
    TS_OPEN_NORMAL,
    TS_OPEN_REQUEST,
    TS_OPEN_RESTORED,
    TS_CLOSED_NORMAL,
    TS_CLOSED_PART,
    TS_CLOSED_BY,
    TS_DELETED = 6,
    TS_CLOSE_REQUEST,
    TS_DELETE_REQUEST,
    TS_CANCEL_REQUEST,
    TS_FINANCE_OPEN_REQUEST,
    TS_FINANCE_UPDATE_REQUEST,
    TS_FINANCE_DELETE_REQUEST = 12,
    TS_RESTORE_REQUEST,
    TS_OPEN_UPDATE_REQUEST,
    TS_CLOSED_UPDATE_REQUEST,
    TS_PENDING_UPDATE_REQUEST
};

enum {
    TRD_ALL_TRADES = 0,
    TRD_OPEN_TRADES,
    TRD_CLOSE_TRADES,
    TRD_PENDING_TRADES,
    TRD_FINANCE_TRADES
};

enum {
    TR_REASON_CLIENT = 0,
    TR_REASON_EXPERT = 1,
    TR_REASON_DEALER = 2,
    TR_REASON_SIGNAL = 3,
    TR_REASON_GATEWAY = 4,
    TR_REASON_MOBILE = 5,
    TR_REASON_WEB = 6,
    TR_REASON_API = 7
};

enum {
    REPORT_NONE_TYPE,                    //report without filter
    REPORT_RANGE_TYPE,                   //report filter with time range
    REPORT_DAILY_TYPE,                   //report filter by one day
    REPORT_ACCOUNT_TYPE,                 //report filter by account
    REPORT_SYMBOL_TYPE,                  //report filter by account
    REPORT_GROUP_TYPE,                   //report filter with group mask
    REPORT_RANGE_GROUP_TYPE,             //report filter one day and group mask
    REPORT_DAILY_GROUP_TYPE,             //report filter one day and group mask
    REPORT_RANGE_ACCOUNT_TYPE,           //report filter with time range and account
    REPORT_DAILY_ACCOUNT_TYPE,           //report filter by one day and account
    REPORT_RANGE_SYMBOL_TYPE,            //report filter with time range and account
    REPORT_DAILY_SYMBOL_TYPE,            //report filter by one day and account
    REPORT_RANGE_GROUP_SYMBOL_TYPE,      //report filter with time range and group and symbols
    REPORT_DAILY_GROUP_SYMBOL_TYPE       //report filter by one day and groups and symbols
};

enum {
    ACTIVATION_NONE = 0,
    ACTIVATION_SL, ACTIVATION_TP,
    ACTIVATION_PENDING,
    ACTIVATION_STOPOUT,
    ACTIVATION_SL_ROLLBACK = -ACTIVATION_SL,
    ACTIVATION_TP_ROLLBACK = -ACTIVATION_TP,
    ACTIVATION_PENDING_ROLLBACK = -ACTIVATION_PENDING,
    ACTIVATION_STOPOUT_ROLLBACK = -ACTIVATION_STOPOUT
};

struct TradeRecord {
    int order = 0;
    int login;
    std::string symbol;
    int digits = 2;
    int cmd;
    int volume;

    time_t open_time;
    int state;
    double open_price;
    double sl, tp;
    double margin_initial;
    time_t close_time;

    int gw_volume;
    time_t expiration;
    int reason;

    std::array<double, 2> conv_rates;

    double commission;
    double prev_commission = 0.0;
    double commission_agent;
    double storage;
    double prev_storage = 0.0;
    double profit;
    double prev_profit = 0.0;

    double close_price;
    double taxes;

    int magic;
    std::string comment;
    int gw_order;
    int activation;
    double gw_open_price;
    double gw_close_price;
    double margin_rate;
    std::string api_data;
    time_t timestamp;

    int db_state = DbStateType::DB_NO_CHANGE;
};

struct TradeDiffRecord {
    int volume;
    time_t open_time;
    int state;
    double open_price;
    double sl, tp;
    time_t close_time;
    time_t expiration;
    int reason;
    double commission;
    double storage;
    double profit;
    double close_price;
    double taxes;
    int magic;
    std::string comment;
    int gw_order;
    int activation;
    double gw_open_price;
    double gw_close_price;
    time_t timestamp;
};

struct TradeSummaryRecord {
    std::string symbol;
    int digits = 0;
    int type = 0;

    int orders = 0;
    int buylots = 0;
    int selllots = 0;
    double buyprice = 0.0;
    double sellprice = 0.0;
    double profit = 0.0;

    int covorders = 0;
    int covbuylots = 0;
    int covselllots = 0;
    double covbuyprice = 0.0;
    double covsellprice = 0.0;
    double covprofit = 0.0;
};

enum { EXE_REQUEST, EXE_INSTANT, EXE_MARKET };

enum { TRADE_NO, TRADE_CLOSE, TRADE_FULL };

enum { SWAP_BY_POINTS, SWAP_BY_DOLLARS, SWAP_BY_INTEREST, SWAP_BY_MARGIN_CURRENCY };

enum { PROFIT_CALC_FOREX, PROFIT_CALC_CFD, PROFIT_CALC_FUTURES };

enum { MARGIN_CALC_FOREX, MARGIN_CALC_CFD, MARGIN_CALC_FUTURES, MARGIN_CALC_CFDINDEX, MARGIN_CALC_CFDLEVERAGE };

enum { ORDERS_DAILY, ORDERS_GTC, ORDERS_DAILY_NO_STOPS };

enum { MARKET_IS_OPEN, MARKET_IS_CLOSE };

struct SymbolGroupRecord {
    int sec_index = 0;
    int parent_sec_index = -1;
    std::string name = "Reserve";
    std::string description = "Reserve";
    int trade = 1;
    int show = 1;
    int sort_index = 0;
};

struct ConSession {
    short open_hour, open_min;
    short close_hour, close_min;
    int open, close;
    short align[7];
};

struct ConSessions {
    ConSession quote[3];
    ConSession trade[3];
    int quote_overnight;
    int trade_overnight;
    int reserved[2];
};

struct SymbolRecord {
    std::string symbol;
    std::string description;
    std::string source;
    std::string currency;
    int sec_index = 0;
    int digits = 0;
    int trade = 0;
    int sym_index = 0;
    int sort_index = 0;

    std::string background_color = "#ffffff";
    int count = 0;
    int count_original = 0;

    ConSessions sessions[7];

    int profit_mode = 0;
    int profit_reserved = 0;

    int filter = 0;
    int filter_counter = 0;
    double filter_limit = 0.0;
    int filter_smoothing = 0;
    float filter_reserved = 0.0f;
    int logging = 0;

    int spread = 0;
    int spread_balance = 0;

    int exemode = 0;
    int swap_enable = 0;
    int swap_type = 0;
    double swap_long = 0.0, swap_short = 0.0;
    int swap_rollover3days = 0;

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

    time_t lasttime = 0;
    double bid = 0.0, ask = 0.0;

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

    int unused[21]{};

    int db_state = DbStateType::DB_NO_CHANGE;
};

struct SymbolAdditionalRecord {
    std::string symbol;
    std::string clear_symbol;
    std::string full_symbol_name;
    std::string exchange;
    std::string description;
    std::string category;
    std::string market;
    std::string sector;
    std::string country;
    std::string industry;
    std::string icon;
    int shareholders = 0;
    double market_cap = 0.0;
    double analyst_rating = 0.0;
    double os_rating = 0.0;
    double ma_rating = 0.0;
    double tech_rating = 0.0;
};

enum {
    SESSION_TYPE_TCP = 0,
    SESSION_TYPE_FIX
};

enum { EXECUTION_MANUAL, EXECUTION_AUTO, EXECUTION_ACTIVITY };

enum { COMM_TYPE_MONEY, COMM_TYPE_PIPS, COMM_TYPE_PERCENT };

enum { COMMISSION_PER_LOT, COMMISSION_PER_DEAL };

enum { TRADE_DENY_NONE = 0, TRADE_DENY_CLOSEBY = 1, TRADE_DENY_MUCLOSEBY = 2 };

enum {
    CLOSE_OUT_NONE, CLOSE_OUT_HIHI, CLOSE_OUT_LOLO, CLOSE_OUT_HILO, CLOSE_OUT_LOHI, CLOSE_OUT_FIFO, CLOSE_OUT_LIFO,
    CLOSE_OUT_INTRDAY_FIFO
};

enum { MARGIN_MODE_DONT_USE, MARGIN_MODE_USE_ALL, MARGIN_MODE_USE_PROFIT, MARGIN_MODE_USE_LOSS };

enum { MARGIN_TYPE_PERCENT, MARGIN_TYPE_CURRENCY };

enum { MARGINLEVEL_OK = 0, MARGINLEVEL_MARGINCALL, MARGINLEVEL_STOPOUT };

enum { NEWS_NO, NEWS_TOPICS, NEWS_FULL };

enum {
    ALLOW_FLAG_EMAIL = 1, ALLOW_FLAG_TRAILING = 2, ALLOW_FLAG_ADVISOR = 4, ALLOW_FLAG_EXPIRATION = 8,
    ALLOW_FLAG_SIGNALS_ALL = 16, ALLOW_FLAG_SIGNALS_OWN = 32, ALLOW_FLAG_RISK_WARNING = 64,
    ALLOW_FLAG_FORCED_OTP_USAGE = 128
};

enum { OTP_MODE_DISABLED = 0, OTP_MODE_TOTP_SHA256 = 1 };

struct MarginLevel {
    int login;
    std::string group;
    int leverage;
    double balance = 0.0;
    double credit = 0.0;
    double equity = 0.0;
    double profit = 0.0;
    double storage = 0.0;
    double commission = 0.0;
    double margin = 0.0;
    double margin_free = 0.0;
    double margin_level = 0.0;
    int margin_type = MARGIN_TYPE_PERCENT;
    int level_type = MARGINLEVEL_OK;
};

struct AccountRecord {
    int login = 0;                     // Логин пользователя (уникальный идентификатор)
    std::string group;                 // Группа пользователя (например, "Admin", "Trader")
    std::string password;              // Пароль (обычно в зашифрованном виде)

    int enable = 1;                     // Статус учетной записи (1 - активна, 0 - заблокирована)
    int enable_change_password;         // Разрешено ли менять пароль (1 - да, 0 - нет)
    int enable_read_only;               // Режим "Только просмотр" (1 - нельзя торговать)
    int enable_otp;                     // Включена ли двухфакторная аутентификация (OTP)
    int enable_reserved[2];             // Зарезервированные параметры

    std::string password_investor;      // Пароль инвестора (режим "Только просмотр")
    std::string password_phone;         // Пароль для подтверждения операций по телефону

    std::string name;                   // Полное имя пользователя
    std::string country;                // Страна проживания
    std::string city;                   // Город проживания
    std::string state;                  // Область/штат
    std::string zipcode;                // Почтовый индекс
    std::string address;                // Адрес проживания
    std::string lead_source;            // Источник привлечения клиента (например, "Реклама", "Партнер")
    std::string phone;                  // Контактный телефон
    std::string email;                  // Электронная почта
    std::string comment;                // Дополнительный комментарий к пользователю
    std::string id;                     // Идентификационный номер (паспорт, ИНН)
    std::string status;                 // Статус клиента (например, "Active", "VIP", "Blocked")

    time_t regdate;                     // Дата регистрации пользователя в системе
    time_t lastdate;                    // Дата последнего входа в систему

    int leverage;                           // Кредитное плечо (например, 1:100, 1:500) // только для форекс
    int agent_account;                      // ID реферального агента (если есть)
    time_t timestamp;                       // Временная метка последнего обновления данных
    int last_ip;                            // Последний IP-адрес входа (в числовом формате)

    double balance;                         // Текущий баланс пользователя дублирующий он равен сумме позиций
    double prevmonthbalance;                // Баланс на конец предыдущего месяца
    double prevbalance;                     // Баланс на конец предыдущего дня

    double credit;                          // Кредитные средства (если были предоставлены)
    double interestrate;                    // Процентная ставка (например, по депозиту)
    double taxes;                           // Налоговые удержания
    double prevmonthequity;                 // Эквити (чистые средства) на конец предыдущего месяца
    double prevequity;                      // Эквити на конец предыдущего дня
    double reserved2[2];                    // Зарезервированные значения для будущего использования

    std::string otp_secret;                 // Секретный ключ для генерации одноразовых паролей (OTP)
    std::string secure_reserved;            // Зарезервированные параметры безопасности
    int send_reports;                       // Флаг отправки отчетов на e-mail (1 - да, 0 - нет)
    int mqid;                               // Идентификатор в системе MQ

    std::string user_color = "#ffffff";        // Цветовая метка пользователя в системе (например, для VIP-клиентов)

    std::string unused;                          // Зарезервированные данные (на будущее расширение системы)
    std::string api_data;                        // Данные для API-интеграции
    MarginLevel margin;                          // todo как идея

    int db_state = DbStateType::DB_NO_CHANGE;
};



struct AccountDiffRecord {
    int login = 0;
    std::string group;

    int enable = 1;
    int enable_change_password;
    int enable_read_only;
    int enable_otp;

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

    time_t regdate;
    time_t lastdate;

    int leverage;
    int agent_account;

    double balance;
    double prevmonthbalance;
    double prevbalance;

    double credit;
    double interestrate;
    double taxes;
    double prevmonthequity;
    double prevequity;

    std::string otp_secret;
    std::string secure_reserved;
    int send_reports;

    std::string user_color = "#ffffff";
};

struct GroupRecordMargin {
    std::string symbol;
    int swap_enable = 0;
    double swap_long = 0.0;
    double swap_short = 0.0;

    double margin_divider = 0.0;

    int spread_enable = 0;
    int spread = 0;
    int spread_balance = 0;

    int reserved[7]{};
};

struct GroupRecordSec {
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
    int lot_min = 0, lot_max = 0;
    int lot_step = 0;
    int ie_deviation = 0;
    int confirmation = 0;
    int trade_rights = 0;
    int ie_quick_mode = 0;
    int autocloseout_mode = 0;
    double comm_tax = 0.0;
    int comm_agent_lots = 0;
    int freemargin_mode = 0;
    int reserved[3]{};
};

struct GroupRecord {
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

    std::unordered_map<int, GroupRecordSec> secgroups;
    std::unordered_map<std::string, GroupRecordMargin> secmargins;

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

    unsigned int news_languages[8]{};
    unsigned int news_languages_total = 0;

    int reserved[17]{};
};

struct CandleRecord {
    double open = 0;
    double high = 0;
    double low = 0;
    double close = 0;
    time_t time = 0;
    double volume = 0;
};

struct CServerInterface {
    virtual int TickSet(TickInfo& tick); //set quotes tick
    virtual int LogsOut(const std::string& type, const std::string& message);  //send logs to console
    static int GetApiVersion() { return PLUGIN_SERVER_API; }

    //+------------------------------------------------------------------+
    // Accounts
    //+------------------------------------------------------------------+
    virtual int GetAccountsByGroup(const std::string& group, std::vector<AccountRecord>* accounts); //Get acccounts by group
    virtual int GetAccountByLogin(int login, AccountRecord* account);                               //Get acccount by login
    virtual int GetAccountBalanceByLogin(int login, MarginLevel* margin);                           //Get acccount by login
    virtual int AddAccount(const AccountRecord& account);                                           //Add acccount
    virtual int UpdateAccount(const AccountRecord& account);                                        //Upd acccount
    virtual int DeleteAccount(int login);                                                           //Del acccount by login

    //+------------------------------------------------------------------+
    // Trades
    //+------------------------------------------------------------------+
    virtual int OpenTrade(const TradeRecord& trade);
    virtual int CloseTrade(const TradeRecord& trade);
    virtual int UpdateOpenTrade(const TradeRecord& trade);
    virtual int UpdateCloseTrade(const TradeRecord& trade);
    virtual int CheckOpenTrade(const TradeRecord& trade);
    virtual int CheckCloseTrade(const TradeRecord& trade);
    virtual int GetOpenTradesByLogin(int login, std::vector<TradeRecord>* trades);
    virtual int GetOpenTradesByMagic(int magic, std::vector<TradeRecord>* trades);
    virtual int GetOpenTradeByOrder(int order, TradeRecord* trade);
    virtual int GetOpenTradesByGroup(const std::string& group_name, time_t from, time_t to, std::vector<TradeRecord>* trades);
    virtual int GetCloseTradesByLogin(int login, std::vector<TradeRecord>* trades);
    virtual int GetCloseTradesByGroup(const std::string& group_name, time_t from, time_t to, std::vector<TradeRecord>* trades);
    virtual int GetAllOpenTrades(std::vector<TradeRecord>* trades);

    //+------------------------------------------------------------------+
    // Finance
    //+------------------------------------------------------------------+
    virtual int BalanceIn(int login, double amount, const std::string& comment);
    virtual int BalanceOut(int login, double amount, const std::string& comment);
    virtual int CreditIn(int login, double amount, const std::string& comment);
    virtual int CreditOut(int login, double amount, const std::string& comment);
    virtual int GetTransactionsByGroup(const std::string& group_name, time_t from, time_t to, std::vector<TradeRecord>* trades);

    //+------------------------------------------------------------------+
    // Symbols
    //+------------------------------------------------------------------+
    virtual int GetSymbol(const std::string& symbol, SymbolRecord *cs);

    //+------------------------------------------------------------------+
    // Groups
    //+------------------------------------------------------------------+
    virtual int GetGroup(const std::string& group_name, GroupRecord* group);
    virtual int GetAllGroups(std::vector<GroupRecord>* groups);

    //+------------------------------------------------------------------+
    // System
    //+------------------------------------------------------------------+
    virtual int CalculateCommission(const TradeRecord& trade, double *calculated_commission);
    virtual int CalculateSwap(const TradeRecord& trade, double *calculated_swap);
    virtual int CalculateProfit(const TradeRecord& trade, double *calculated_profit);
    virtual int CalculateMargin(const TradeRecord& trade, double *calculated_margin);
    virtual int CalculateConvertRateByCurrency(const std::string& from_cur, const std::string& to_cur, int cmd, double *multiplier);

    //+------------------------------------------------------------------+
    // Chart
    //+------------------------------------------------------------------+
    virtual int GetCandles(const std::string& symbol, const std::string& frame, time_t from, time_t to, std::vector<CandleRecord>* candles);
    virtual int SetCandles(const std::string& symbol, const std::vector<CandleRecord>& candles);
    virtual int DeleteCandlesAll(const std::string& symbol);
    virtual int DeleteCandlesPeriod(const std::string& symbol, time_t from, time_t to);


    //+------------------------------------------------------------------+
    // Stream
    //+------------------------------------------------------------------+
    virtual int SendToManager(int manager_id, const Value & data);//send data to some Manager
    virtual int BroadcastToManagers(const Value & data); //send data to all Managers
    virtual int SendToAccount(int account_id, const Value & data); //send data to some Account
    virtual int BroadcastToAccounts(const Value & data); //send data to all Accounts
    virtual int SendState(const Value & data); //send update data to Plugin State
};

#endif