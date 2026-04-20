//
// Created by  ScaleTrade on 17.03.2025.
//
#pragma once

#include <ctime>
#include <string>

//+------------------------------------------------------------------+
//|                           Result codes                           |
//+------------------------------------------------------------------+
enum ReturnCodes {
   //--- common errors
    RET_OK                =0,        // all OK
    RET_OK_NONE,                     // all OK-no operation
    RET_ERROR,                       // general error
    RET_INVALID_DATA,                // invalid data
    RET_TECH_PROBLEM,                // server technical problem
    RET_OLD_VERSION,                 // old client terminal
    RET_NO_CONNECT,                  // no connection
    RET_NOT_ENOUGH_RIGHTS,           // no enough rights
    RET_TOO_FREQUENT,                // too frequently access to server
    RET_MALFUNCTION,                 // mulfunctional operation
    RET_GENERATE_KEY,                // need to send public key
    RET_SECURITY_SESSION,            // security session start
    RET_INVALID_PASSWORD,            // invalid password
    RET_ERR_PARAMS,                  // invalid params
    RET_ERR_DATA,                   // Invalid data"
    RET_ERR_DISK,                   // Disk error"
    RET_ERR_MEM,                    // Memory error"
    RET_ERR_NETWORK,                // Network error"
    RET_ERR_PERMISSIONS,            // Not enough permissions"
    RET_ERR_TIMEOUT,                // Operation timeout"
    RET_ERR_CONNECTION,             // No connection"
    RET_ERR_NOSERVICE,              // Service is not available"
    RET_ERR_FREQUENT,               // Too frequent requests"
    RET_ERR_NOTFOUND,               // Not found"
    RET_ERR_SHUTDOWN,               // Server shutdown in progress"
    RET_ERR_CANCEL,                 // Operation was canceled"
    RET_ERR_DUPLICATE,              // Duplicate attempt"
    RET_OK_CHANGE,                  // Return Ok change some field"

    //--- Groups errors
    RET_GROUP_NOT_FOUND = 28,        //  Пользователь не найден
    RET_GROUP_USE_BY_USER,           //  Group use by accounts

    //--- Symobls errors
    RET_SEC_USE_BY_SYMBOL = 32,     // Группа смиволов используется символом
    RET_SYMBOL_USE_BY_TRADE,     // Группа смиволов используется символом
    RET_SYMBOL_NOT_FOUND,     // Группа смиволов используется символом
    RET_SEC_NOT_FOUND,        // Symbol group not found

    //--- Users errors
    RET_USER_NOT_FOUND = 48,            //  Пользователь не найден
    RET_USR_LAST_ADMIN,                 //Last admin account cannot be deleted"
    RET_USR_LOGIN_EXHAUSTED,           //"Login range exhausted"
    RET_USR_LOGIN_PROHIBITED,          //"Login reserved at another server"
    RET_USR_LOGIN_EXIST,               //"Account already exists"
    RET_USR_SUICIDE,                   //"Attempt of self-deletion"
    RET_USR_INVALID_PASSWORD,          //"Invalid account password"
    RET_USR_LIMIT_REACHED,             //"User limit reached"
    RET_USR_HAS_TRADES,                //"Account has open trades"
    RET_USR_DIFFERENT_SERVERS,         //"Attempt to move account to different
    RET_USR_DIFFERENT_CURRENCY,        //"Attempt to move account to group with
    RET_USR_IMPORT_BALANCE,            //"Account balance import error"
    RET_USR_IMPORT_GROUP,              //"Imported account has invalid group
    RET_USR_ACCOUNT_EXIST,             //"Account already exist"

    //--- account status
    RET_ACCOUNT_DISABLED  =64,       // account blocked
    RET_BAD_ACCOUNT_INFO,            // bad account info
    RET_PUBLIC_KEY_MISSING,          // no key

    //--- trade
    RET_TRADE_TIMEOUT     = 128,      // trade transatcion timeou expired
    RET_TRADE_BAD_PRICES,            // order has wrong prices
    RET_TRADE_BAD_STOPS,             // wrong stops level
    RET_TRADE_BAD_VOLUME,            // wrong lot size
    RET_TRADE_MARKET_CLOSED,         // market closed
    RET_TRADE_DISABLE,               // trade disabled
    RET_TRADE_NO_MONEY,              // no enough money for order execution
    RET_TRADE_PRICE_CHANGED,         // price changed
    RET_TRADE_OFFQUOTES,             // no quotes
    RET_TRADE_BROKER_BUSY,           // broker is busy
    RET_TRADE_REQUOTE,               // requote
    RET_TRADE_ORDER_LOCKED,          // order is proceed by dealer and cannot be changed
    RET_TRADE_LONG_ONLY,             // allowed only BUY orders
    RET_TRADE_TOO_MANY_REQ,          // too many requests from one client

    //--- order status notification
    RET_TRADE_ACCEPTED = 145,              // trade request accepted by server and placed in request queue
    RET_TRADE_PROCESS,               // trade request accepted by dealerd
    RET_TRADE_USER_CANCEL,           // trade request canceled by client

    //--- additional return codes
    RET_TRADE_MODIFY_DENIED = 150,         // trade modification denied
    RET_TRADE_CONTEXT_BUSY,          // trade context is busy (used in client terminal)
    RET_TRADE_EXPIRATION_DENIED,     // using expiration date denied
    RET_TRADE_TOO_MANY_ORDERS,       // too many orders
    RET_TRADE_HEDGE_PROHIBITED,      // hedge is prohibited
    RET_TRADE_PROHIBITED_BY_FIFO,    // prohibited by fifo rule
    RET_DUPLICATE_RECORD,            // попытка создание уже существующего трейда
    RET_TRADE_SEC_NOT_FOUND,         // не нашел активную символ-группу (sec) в группе пользователя
    RET_TRADE_INVALID_SL_TP,         // не валидные значения SL TP
    RET_TRADE_BAD_CLOSE_TIME,        // В обработку поступило не валидный параметр Время закрытия
    RET_TRADE_READ_ONLY = 160,       // Аккаунт имеет права только на чтение
    RET_TRADE_INCORRECT_STATE,       // В обработку поступил не валидный state
    RET_TRADE_INCORRECT_CMD,         // В обработку поступил не валидный cmd
    RET_TRADE_INCORRECT_ORDER,       // В обработку поступил не валидный order
    RET_TRADE_EXIST,
    RET_TRADE_LIMIT_REACHED,
    RET_TRADE_HAS_OPEN,             // Данный ордер существует

    RET_FEED_ALREADY_LOAD = 180,     //

    //--- FIX
    RET_FIX_SLOT_NOT_FOUND = 200,
    RET_FIX_SLOT_DISABLED,
    RET_FIX_SLOT_NOT_ASSIGNED,
    RET_FIX_SLOT_OUTSIDE_RUNTIME_POOL,
    RET_FIX_SLOT_PASSWORD_INVALID,
    RET_FIX_SLOT_RULES_DENY_LOGON,
    RET_FIX_SLOT_RUNTIME_LIMIT_REACHED,
    RET_FIX_RUNTIME_SESSION_UNAVAILABLE,

    //--- ошибки/коды ошибок конфига
    RET_CFG_LAST_ADMIN = 256,              //  "Last admin config cannot be deleted" },
    RET_CFG_LAST_ADMIN_GROUP,           //"Last admin group cannot be deleted"  },
    RET_CFG_NOT_EMPTY,                  //"Accounts or trades in group/symbol"  },
    RET_CFG_INVALID_RANGE,              //"Invalid account or trade ranges"},
    RET_CFG_NOT_MANAGER_LOGIN,          //"Account doesn't belong to manager group
    RET_CFG_BUILTIN,                    //"Built-in protected config"        },
    RET_CFG_DUPLICATE,                  //"Configuration duplicate"          },
    RET_CFG_LIMIT_REACHED,              //"Configuration limit reached"      },
    RET_CFG_NO_ACCESS_TO_MAIN,          //"Invalid network configuration"    },
    RET_CFG_DEALER_ID_EXIST,            //"Dealer with same ID already exists"
    RET_CFG_BIND_ADDR_EXIST,            //"Binding address already exists"
    RET_CFG_WORKING_TRADE,              //"Attempt to delete working trade server"
    RET_CFG_GATEWAY_NAME_EXIST,         //"Gateway with same name already exists"
    RET_CFG_SWITCH_TO_BACKUP,           //"Server must be switched to backup mode"
    RET_CFG_NO_BACKUP_MODULE,           //"Backup server module is absent"
    RET_CFG_NO_TRADE_MODULE,            //"Trade server module is absent"
    RET_CFG_NO_HISTORY_MODULE,          //"History server module is absent"
    RET_CFG_ANOTHER_SWITCH,             //"Another switching process in progress"
    RET_CFG_NO_LICENSE_FILE,            //"License file is absent"
    RET_CFG_GATEWAY_LOGIN_EXIST,        //"Gateway with same login already exist"
    RET_LICENSE_NOT_ACTIVE,             //"License not active"

    //--- Понятные ответы
    RET_BAD_REQUEST = 400,       // trade modification denied
    RET_UNAUTHORIZED,            // trade context is busy (used in client termin
    RET_FIELD_REQUIRED,          // using expiration date denied
    RET_FORBIDDEN,               // too many orders
    RET_NOT_FOUND,               // hedge is prohibited
    RET_METHOD_NOT_ALLOWED,      // prohibited by fifo rule
    RET_INVALID_METHOD,
    RET_TIMEOUT                  // timeout
  };


//+------------------------------------------------------------------+
//|                           Event codes                            |
//+------------------------------------------------------------------+

//--- вариант событий
enum EventType {
    EV_TYPE_BALANCE = 0,        // События изменения баланса
    EV_TYPE_TRADE,              // События изменения трейда
    EV_TYPE_SYMBOL,             // События изменения символа
    EV_TYPE_GROUP,              // События изменения трейда
    EV_TYPE_MARKET,             // События изменения рынка
    EV_TYPE_ACCOUNT,            // События изменения аккаунта
    EV_TYPE_SETTINGS,           // События изменения аккаунта
    EV_TYPE_NOTIFY,             // События нотификаций
};

//--- типы событий
enum EventRecordType {
    EV_RECORD_ADD = 0,         // Добавлена новая запись
    EV_RECORD_UPDATE,          // Изменена запись
    EV_RECORD_DELETE,          // Удалена запись
    EV_RECORD_RESTORE,         // Востановлена запись
    EV_RECORD_ARCHIVE,         // Удалена запись
    EV_RECORD_ACTIVATE_TRADE,  // Трейд активировался запись
    EV_RECORD_CLOSE_TRADE      // Трейд закрылся запись
};

//--- Session type
enum {
    SESSION_USER = 0,                       // Изменена запись
    SESSION_MANAGER,                        // Сессия из под менеджера
    SESSION_DEALER,                         // Добавлена новая запись
    SESSION_ADMIN,                          // Сессия из под админа
    SESSION_SYSTEM,                         // Добавлена новая запись
    SESSION_FIX,                            // Добавлена новая запись
    SESSION_CRM_MANAGER = 10,               // Добавлена новая запись
    SESSION_CRM_ADMIN                       // Добавлена новая запись
};

//+------------------------------------------------------------------+
// Type of connections
//+------------------------------------------------------------------+
enum ConnectionType {
    CONNECTION_TYPE_CLIENT_DESKTOP    = 0, // Connection from a client desktop terminal
    CONNECTION_TYPE_CLIENT_WINPHONE   = 1, // Connection from a Windows Phone 7 terminal
    CONNECTION_TYPE_CLIENT_IPHONE     = 2, // Connection from an iPhone terminal
    CONNECTION_TYPE_CLIENT_ANDROID    = 3, // Connection from an Android terminal
    CONNECTION_TYPE_CLIENT_BLACKBERRY = 4, // Connection from a BlackBerry terminal
    CONNECTION_TYPE_CLIENT_WEB        = 5, // Connection from a WebTerminal
    CONNECTION_TYPE_REST              = 6, // Connection via the client REST API
    CONNECTION_TYPE_TCP               = 7, // Connection from an administrator terminal
    CONNECTION_TYPE_FIX               = 8  // Connection from a manager terminal
};


enum DbStateType {
    DB_NO_CHANGE  = 0,
    DB_NEED_INSERT,
    DB_NEED_UPDATE,
    DB_NEED_DELETE,
    DB_NEED_UPSERT
};

// Quotes
//+------------------------------------------------------------------+
//| Symbol tick                                                      |
//+------------------------------------------------------------------+
struct TickInfo
{
    std::string       symbol;                 // symbol
    time_t            ctm;                    // tick time
    double            volume;                 // volume
    double            bid;                    // bid
    double            ask;                    // ask
};

//+------------------------------------------------------------------+
// ReturnCodes description
//+------------------------------------------------------------------+
inline std::string FormatError(const int retcode) {

//--- error description
   struct ErrorDesc
     {
      int          code;
      std::string  desc;
     };
//--- errors
   static ErrorDesc errors[] =
     {
       { RET_OK,                    "OK"                               },
       { RET_OK_NONE,               "OK/None"                          },
       { RET_ERROR,                 "Common error"                     },
       { RET_ERR_PARAMS,            "Invalid parameters"               },
       { RET_INVALID_DATA,          "Invalid parameters"               },
       { RET_ERR_DATA,              "Invalid data"                     },
       { RET_ERR_DISK,              "Disk error"                       },
       { RET_ERR_MEM,               "Memory error"                     },
       { RET_ERR_NETWORK,           "Network error"                    },
       { RET_ERR_PERMISSIONS,       "Not enough permissions"           },
       { RET_ERR_TIMEOUT,           "Operation timeout"                },
       { RET_ERR_CONNECTION,        "No connection"                    },
       { RET_ERR_NOSERVICE,         "Service is not available"         },
       { RET_ERR_FREQUENT,          "Too frequent requests"            },
       { RET_ERR_NOTFOUND,          "Not found"                        },
       { RET_ERR_SHUTDOWN,          "Server shutdown in progress"      },
       { RET_ERR_CANCEL,            "Operation was canceled"           },
      { RET_ERR_DUPLICATE,         "Duplicate attempt"                },

      // //--- symbol errors
      { RET_SEC_USE_BY_SYMBOL,     "Symbol group is used by one or more symbols" },
      { RET_SEC_NOT_FOUND,         "Symbol group not found" },
      { RET_SYMBOL_USE_BY_TRADE,   "The symbol is used in open trades"  },

      //
      // // //--- auth errors
      // { RET_AUTH_CLIENT_INVALID,   L"Invalid terminal type"            },
      // { RET_AUTH_ACCOUNT_INVALID,  L"Invalid account"                  },
      // { RET_AUTH_ACCOUNT_DISABLED, L"Account disabled"                 },
      // { RET_AUTH_ADVANCED,         L"Advanced authorization"           },
      // { RET_AUTH_CERTIFICATE,      L"Certificate required"             },
      // { RET_AUTH_CERTIFICATE_BAD,  L"Invalid certificate"              },
      // { RET_AUTH_NOTCONFIRMED,     L"Certificate is not confirmed"     },
      // { RET_AUTH_SERVER_INTERNAL,  L"Attempt to connect to non-access server" },
      // { RET_AUTH_SERVER_BAD,       L"Invalid or fake server"           },
      // { RET_AUTH_UPDATE_ONLY,      L"Only updates available"           },
      // { RET_AUTH_CLIENT_OLD,       L"Old version"                      },
      // { RET_AUTH_MANAGER_NOCONFIG, L"Account doesn't have manager config" },
      // { RET_AUTH_MANAGER_IPBLOCK,  L"IP address unallowed for manager" },
      // { RET_AUTH_GROUP_INVALID,    L"Group is not initialized, server restart needed" },
      // { RET_AUTH_CA_DISABLED,      L"Certificate generation disabled"  },
      // { RET_AUTH_INVALID_ID,       L"Invalid or disabled server id [check server id]"},
      // { RET_AUTH_INVALID_IP,       L"Unallowed address [check server ip address]"    },
      // { RET_AUTH_INVALID_TYPE,     L"Invalid server type [check server id and type]" },
      // { RET_AUTH_SERVER_BUSY,      L"Server is busy"                   },
      // { RET_AUTH_SERVER_CERT,      L"Invalid server certificate or invalid local time" },
      // { RET_AUTH_ACCOUNT_UNKNOWN,  L"Unknown account"                  },
      // { RET_AUTH_SERVER_OLD,       L"Old server version"               },
      // { RET_AUTH_SERVER_LIMIT,     L"Server cannot be connected due to license limitation" },
      // { RET_AUTH_MOBILE_DISABLED,  L"Mobile terminal isn't allowed in license"             },
      // { RET_AUTH_MANAGER_TYPE,     L"Connection type is not permitted for manager"         },
      // { RET_AUTH_DEMO_DISABLED,    L"Demo allocation disabled"         },
      // { RET_AUTH_RESET_PASSWORD,   L"Master password must be changed"  },

      // //--- config errors
      { RET_CFG_LAST_ADMIN,        "Last admin config cannot be deleted" },
      { RET_CFG_LAST_ADMIN_GROUP,  "Last admin group cannot be deleted"  },
      { RET_CFG_NOT_EMPTY,         "Accounts or trades in group/symbol"  },
      { RET_CFG_INVALID_RANGE,     "Invalid account or trade ranges"},
      { RET_CFG_NOT_MANAGER_LOGIN, "Account doesn't belong to manager group"},
      { RET_CFG_BUILTIN,           "Built-in protected config"        },
      { RET_CFG_DUPLICATE,         "Configuration duplicate"          },
      { RET_CFG_LIMIT_REACHED,     "Configuration limit reached"      },
      { RET_CFG_NO_ACCESS_TO_MAIN, "Invalid network configuration"    },
      { RET_CFG_DEALER_ID_EXIST,   "Dealer with same ID already exists"       },
      { RET_CFG_BIND_ADDR_EXIST,   "Binding address already exists"              },
      { RET_CFG_WORKING_TRADE,     "Attempt to delete working trade server" },
      { RET_CFG_GATEWAY_NAME_EXIST,"Gateway with same name already exists"  },
      { RET_CFG_SWITCH_TO_BACKUP,  "Server must be switched to backup mode" },
      { RET_CFG_NO_BACKUP_MODULE,  "Backup server module is absent"         },
      { RET_CFG_NO_TRADE_MODULE,   "Trade server module is absent"          },
      { RET_CFG_NO_HISTORY_MODULE, "History server module is absent"        },
      { RET_CFG_ANOTHER_SWITCH,    "Another switching process in progress"  },
      { RET_CFG_NO_LICENSE_FILE,   "License file is absent"                 },
      { RET_CFG_GATEWAY_LOGIN_EXIST,"Gateway with same login already exist" },

      // //--- client errors
      { RET_USR_LAST_ADMIN,        "Last admin account cannot be deleted" },
      { RET_USR_LOGIN_EXHAUSTED,   "Login range exhausted"             },
      { RET_USR_LOGIN_PROHIBITED,  "Login reserved at another server"  },
      { RET_USR_LOGIN_EXIST,       "Account already exists"            },
      { RET_USR_SUICIDE,           "Attempt of self-deletion"          },
      { RET_USR_INVALID_PASSWORD,  "Invalid account password"          },
      { RET_USR_LIMIT_REACHED,     "User limit reached"                },
      { RET_USR_HAS_TRADES,        "Account has open trades"           },
      { RET_USR_DIFFERENT_SERVERS, "Attempt to move account to different server"              },
      { RET_USR_DIFFERENT_CURRENCY,"Attempt to move account to group with different currency" },
      { RET_USR_IMPORT_BALANCE,    "Account balance import error"      },
      { RET_USR_IMPORT_GROUP,      "Imported account has invalid group"},
      { RET_USR_ACCOUNT_EXIST,     "Account already exist"             },

      // //--- Group errors
       {RET_GROUP_USE_BY_USER,           "Group use by active accounts"      },
       // {RET_GROUP_BAD_STOPS,             "wrong stops level"},

      // //--- trade errors
       { RET_TRADE_BAD_PRICES,           "Order price its not correct"      },
       {RET_TRADE_BAD_STOPS,             "wrong stops level"},
       {RET_TRADE_BAD_VOLUME,            "wrong lot size"},
       {RET_TRADE_MARKET_CLOSED,         "market closed"},
       {RET_TRADE_DISABLE,               "Trade disabled"},
       {RET_TRADE_NO_MONEY,              "No enough money for order execution"},
       {RET_TRADE_PRICE_CHANGED,         "Price changed"},
       {RET_TRADE_OFFQUOTES,             "No quotes"},
       {RET_TRADE_BROKER_BUSY,           "Broker is busy"},
       {RET_TRADE_REQUOTE,               "Requote"},
       {RET_TRADE_ORDER_LOCKED,          "Order is proceed by dealer and cannot be changed"},
       {RET_TRADE_LONG_ONLY,             "Allowed only BUY orders"},
       {RET_TRADE_TOO_MANY_REQ,          "Too many requests from one client"},

       //--- order status notification
       {RET_TRADE_ACCEPTED,              "Trade request accepted by server and placed in request queue"},
       {RET_TRADE_PROCESS,               "Trade request accepted by dealerd"},
       {RET_TRADE_USER_CANCEL,           "Trade request canceled by client"},
       //--- additional return codes
       {RET_TRADE_MODIFY_DENIED,         "Trade modification denied"},
       {RET_TRADE_CONTEXT_BUSY,          "Trade context is busy (used in client terminal)"},
       {RET_TRADE_EXPIRATION_DENIED,     "Using expiration date denied"},
       {RET_TRADE_TOO_MANY_ORDERS,       "Too many orders"},
       {RET_TRADE_HEDGE_PROHIBITED,      "Hedge is prohibited"},
       {RET_TRADE_PROHIBITED_BY_FIFO,    "Prohibited by fifo rule"},
       {RET_DUPLICATE_RECORD,            "Попытка создание уже существующего трейда"},
       {RET_TRADE_SEC_NOT_FOUND,         "Не нашел активную символ-группу (sec) в группе пользователя"},
       {RET_TRADE_INVALID_SL_TP,         "Invalid SL or TP"},
       {RET_TRADE_EXIST,                 "Trade exist"},
       {RET_TRADE_LIMIT_REACHED,         "Trades limit reached"},
       {RET_TRADE_BAD_CLOSE_TIME,         "An invalid Close Time parameter was provided for trade processing."},

       //--- fix errors
       {RET_FIX_SLOT_NOT_FOUND,           "FIX slot not found"},
       {RET_FIX_SLOT_DISABLED,            "FIX slot disabled"},
       {RET_FIX_SLOT_NOT_ASSIGNED,        "FIX slot is not assigned"},
       {RET_FIX_SLOT_OUTSIDE_RUNTIME_POOL,"FIX slot is outside runtime pool"},
       {RET_FIX_SLOT_PASSWORD_INVALID,    "Invalid FIX slot password"},
       {RET_FIX_SLOT_RULES_DENY_LOGON,    "FIX logon denied by slot rules"},
       {RET_FIX_SLOT_RUNTIME_LIMIT_REACHED,"No free FIX slots available in runtime pool"},
       {RET_FIX_RUNTIME_SESSION_UNAVAILABLE,"FIX runtime session is unavailable"},

      // { RET_TRADE_ORDER_EXIST,     L"Order already exists"             },
      // { RET_TRADE_ORDER_EXHAUSTED, L"Order range exhausted"            },
      // { RET_TRADE_DEAL_EXHAUSTED,  L"Deal range exhausted"             },
      // { RET_TRADE_MAX_MONEY,       L"Money limit reached"              },
      // { RET_TRADE_DEAL_EXIST,      L"Deal already exists"              },
      // { RET_TRADE_ORDER_PROHIBITED,L"Order ticket reserved at another server" },
      // { RET_TRADE_DEAL_PROHIBITED, L"Deal ticket reserved at another server"  },

      // //--- report errors
      // { RET_REPORT_SNAPSHOT,       L"Base snapshot error"              },
      // { RET_REPORT_NOTSUPPORTED,   L"Method is not supported by this report" },
      // { RET_REPORT_NODATA,         L"No data for report"               },
      // { RET_REPORT_TEMPLATE_BAD,   L"Bad template"                     },
      // { RET_REPORT_TEMPLATE_END,   L"End of template"                  },
      // { RET_REPORT_INVALID_ROW,    L"Invalid row size"                 },
      // { RET_REPORT_LIMIT_REPEAT,   L"Tag repeat limit reached "        },
      // { RET_REPORT_LIMIT_REPORT,   L"Report size limit reached"        },

      // //--- history errors
      // { RET_HST_SYMBOL_NOTFOUND,   L"Symbol not found, try to restart history server" },

       //--- ошибки/коды ошибок конфига
     {RET_CFG_LAST_ADMIN,                 "Last admin config cannot be deleted" },
     {RET_CFG_LAST_ADMIN_GROUP,           "Last admin group cannot be deleted"  },
     {RET_CFG_NOT_EMPTY,                  "Accounts or trades in group/symbol"  },
     {RET_CFG_INVALID_RANGE,              "Invalid account or trade ranges"},
     {RET_CFG_NOT_MANAGER_LOGIN,          "Account doesn't belong to manager group"},
     {RET_CFG_BUILTIN,                    "Built-in protected config"        },
     {RET_CFG_DUPLICATE,                  "Configuration duplicate"          },
     {RET_CFG_LIMIT_REACHED,              "Configuration limit reached"      },
     {RET_CFG_NO_ACCESS_TO_MAIN,          "Invalid network configuration"    },
     {RET_CFG_DEALER_ID_EXIST,            "Dealer with same ID already exists"},
     {RET_CFG_BIND_ADDR_EXIST,            "Binding address already exists"},
     {RET_CFG_WORKING_TRADE,              "Attempt to delete working trade server"},
     {RET_CFG_GATEWAY_NAME_EXIST,         "Gateway with same name already exists"},
     {RET_CFG_SWITCH_TO_BACKUP,           "Server must be switched to backup mode"},
     {RET_CFG_NO_BACKUP_MODULE,           "Backup server module is absent"},
     {RET_CFG_NO_TRADE_MODULE,            "Trade server module is absent"},
     {RET_CFG_NO_HISTORY_MODULE,          "History server module is absent"},
     {RET_CFG_ANOTHER_SWITCH,             "Another switching process in progress"},
     {RET_CFG_NO_LICENSE_FILE,            "License file is absent"},
     {RET_CFG_GATEWAY_LOGIN_EXIST,        "Gateway with same login already exist"},
     {RET_LICENSE_NOT_ACTIVE,        "Your license has expired! Please renew it to continue using the trading platform."},

      // //--- API retcodes
      // { RET_ERR_NOTIMPLEMENT,     	   L"Not implemented"                },
      // { RET_ERR_NOTMAIN,                L"Operation must be performed on main server"},
      // { RET_ERR_NOTSUPPORTED,           L"Command doesn't supported"                 },
      // { RET_ERR_DEADLOCK,               L"Operation canceled due possible deadlock"  },
      // { RET_ERR_LOCKED,                 L"Operation on locked entity"                },
     };
//--- search description
    for(auto & error : errors)
        if(error.code==retcode) return(error.desc);
//--- default value

   return "Unknown error";
  }
inline std::string FormatErrorCode(const int retcode) {
    struct ErrorDescCode {
        int         code;
        std::string desc;
    };

    static const ErrorDescCode errors[] = {
        { RET_OK, "RET_OK" },
        { RET_OK_NONE, "RET_OK_NONE" },
        { RET_ERROR, "RET_ERROR" },
        { RET_INVALID_DATA, "RET_INVALID_DATA" },
        { RET_TECH_PROBLEM, "RET_TECH_PROBLEM" },
        { RET_OLD_VERSION, "RET_OLD_VERSION" },
        { RET_NO_CONNECT, "RET_NO_CONNECT" },
        { RET_NOT_ENOUGH_RIGHTS, "RET_NOT_ENOUGH_RIGHTS" },
        { RET_TOO_FREQUENT, "RET_TOO_FREQUENT" },
        { RET_MALFUNCTION, "RET_MALFUNCTION" },
        { RET_GENERATE_KEY, "RET_GENERATE_KEY" },
        { RET_SECURITY_SESSION, "RET_SECURITY_SESSION" },
        { RET_INVALID_PASSWORD, "RET_INVALID_PASSWORD" },
        { RET_ERR_PARAMS, "RET_ERR_PARAMS" },
        { RET_ERR_DATA, "RET_ERR_DATA" },
        { RET_ERR_DISK, "RET_ERR_DISK" },
        { RET_ERR_MEM, "RET_ERR_MEM" },
        { RET_ERR_NETWORK, "RET_ERR_NETWORK" },
        { RET_ERR_PERMISSIONS, "RET_ERR_PERMISSIONS" },
        { RET_ERR_TIMEOUT, "RET_ERR_TIMEOUT" },
        { RET_ERR_CONNECTION, "RET_ERR_CONNECTION" },
        { RET_ERR_NOSERVICE, "RET_ERR_NOSERVICE" },
        { RET_ERR_FREQUENT, "RET_ERR_FREQUENT" },
        { RET_ERR_NOTFOUND, "RET_ERR_NOTFOUND" },
        { RET_ERR_SHUTDOWN, "RET_ERR_SHUTDOWN" },
        { RET_ERR_CANCEL, "RET_ERR_CANCEL" },
        { RET_ERR_DUPLICATE, "RET_ERR_DUPLICATE" },
        { RET_OK_CHANGE, "RET_OK_CHANGE" },

        { RET_GROUP_NOT_FOUND, "RET_GROUP_NOT_FOUND" },
        { RET_GROUP_USE_BY_USER, "RET_GROUP_USE_BY_USER" },

        { RET_SEC_USE_BY_SYMBOL, "RET_SEC_USE_BY_SYMBOL" },
        { RET_SYMBOL_USE_BY_TRADE, "RET_SYMBOL_USE_BY_TRADE" },
        { RET_SYMBOL_NOT_FOUND, "RET_SYMBOL_NOT_FOUND" },
        { RET_SEC_NOT_FOUND, "RET_SEC_NOT_FOUND" },

        { RET_USER_NOT_FOUND, "RET_USER_NOT_FOUND" },
        { RET_USR_LAST_ADMIN, "RET_USR_LAST_ADMIN" },
        { RET_USR_LOGIN_EXHAUSTED, "RET_USR_LOGIN_EXHAUSTED" },
        { RET_USR_LOGIN_PROHIBITED, "RET_USR_LOGIN_PROHIBITED" },
        { RET_USR_LOGIN_EXIST, "RET_USR_LOGIN_EXIST" },
        { RET_USR_SUICIDE, "RET_USR_SUICIDE" },
        { RET_USR_INVALID_PASSWORD, "RET_USR_INVALID_PASSWORD" },
        { RET_USR_LIMIT_REACHED, "RET_USR_LIMIT_REACHED" },
        { RET_USR_HAS_TRADES, "RET_USR_HAS_TRADES" },
        { RET_USR_DIFFERENT_SERVERS, "RET_USR_DIFFERENT_SERVERS" },
        { RET_USR_DIFFERENT_CURRENCY, "RET_USR_DIFFERENT_CURRENCY" },
        { RET_USR_IMPORT_BALANCE, "RET_USR_IMPORT_BALANCE" },
        { RET_USR_IMPORT_GROUP, "RET_USR_IMPORT_GROUP" },
        { RET_USR_ACCOUNT_EXIST, "RET_USR_ACCOUNT_EXIST" },

        { RET_ACCOUNT_DISABLED, "RET_ACCOUNT_DISABLED" },
        { RET_BAD_ACCOUNT_INFO, "RET_BAD_ACCOUNT_INFO" },
        { RET_PUBLIC_KEY_MISSING, "RET_PUBLIC_KEY_MISSING" },

        { RET_TRADE_TIMEOUT, "RET_TRADE_TIMEOUT" },
        { RET_TRADE_BAD_PRICES, "RET_TRADE_BAD_PRICES" },
        { RET_TRADE_BAD_STOPS, "RET_TRADE_BAD_STOPS" },
        { RET_TRADE_BAD_VOLUME, "RET_TRADE_BAD_VOLUME" },
        { RET_TRADE_MARKET_CLOSED, "RET_TRADE_MARKET_CLOSED" },
        { RET_TRADE_DISABLE, "RET_TRADE_DISABLE" },
        { RET_TRADE_NO_MONEY, "RET_TRADE_NO_MONEY" },
        { RET_TRADE_PRICE_CHANGED, "RET_TRADE_PRICE_CHANGED" },
        { RET_TRADE_OFFQUOTES, "RET_TRADE_OFFQUOTES" },
        { RET_TRADE_BROKER_BUSY, "RET_TRADE_BROKER_BUSY" },
        { RET_TRADE_REQUOTE, "RET_TRADE_REQUOTE" },
        { RET_TRADE_ORDER_LOCKED, "RET_TRADE_ORDER_LOCKED" },
        { RET_TRADE_LONG_ONLY, "RET_TRADE_LONG_ONLY" },
        { RET_TRADE_TOO_MANY_REQ, "RET_TRADE_TOO_MANY_REQ" },

        { RET_TRADE_ACCEPTED, "RET_TRADE_ACCEPTED" },
        { RET_TRADE_PROCESS, "RET_TRADE_PROCESS" },
        { RET_TRADE_USER_CANCEL, "RET_TRADE_USER_CANCEL" },

        { RET_TRADE_MODIFY_DENIED, "RET_TRADE_MODIFY_DENIED" },
        { RET_TRADE_CONTEXT_BUSY, "RET_TRADE_CONTEXT_BUSY" },
        { RET_TRADE_EXPIRATION_DENIED, "RET_TRADE_EXPIRATION_DENIED" },
        { RET_TRADE_TOO_MANY_ORDERS, "RET_TRADE_TOO_MANY_ORDERS" },
        { RET_TRADE_HEDGE_PROHIBITED, "RET_TRADE_HEDGE_PROHIBITED" },
        { RET_TRADE_PROHIBITED_BY_FIFO, "RET_TRADE_PROHIBITED_BY_FIFO" },
        { RET_DUPLICATE_RECORD, "RET_DUPLICATE_RECORD" },
        { RET_TRADE_SEC_NOT_FOUND, "RET_TRADE_SEC_NOT_FOUND" },
        { RET_TRADE_INVALID_SL_TP, "RET_TRADE_INVALID_SL_TP" },
        { RET_TRADE_BAD_CLOSE_TIME, "RET_TRADE_BAD_CLOSE_TIME" },
        { RET_TRADE_READ_ONLY, "RET_TRADE_READ_ONLY" },
        { RET_TRADE_INCORRECT_STATE, "RET_TRADE_INCORRECT_STATE" },
        { RET_TRADE_INCORRECT_CMD, "RET_TRADE_INCORRECT_CMD" },
        { RET_TRADE_INCORRECT_ORDER, "RET_TRADE_INCORRECT_ORDER" },
        { RET_TRADE_EXIST, "RET_TRADE_EXIST" },
        { RET_TRADE_LIMIT_REACHED, "RET_TRADE_LIMIT_REACHED" },
        { RET_FIX_SLOT_NOT_FOUND, "RET_FIX_SLOT_NOT_FOUND" },
        { RET_FIX_SLOT_DISABLED, "RET_FIX_SLOT_DISABLED" },
        { RET_FIX_SLOT_NOT_ASSIGNED, "RET_FIX_SLOT_NOT_ASSIGNED" },
        { RET_FIX_SLOT_OUTSIDE_RUNTIME_POOL, "RET_FIX_SLOT_OUTSIDE_RUNTIME_POOL" },
        { RET_FIX_SLOT_PASSWORD_INVALID, "RET_FIX_SLOT_PASSWORD_INVALID" },
        { RET_FIX_SLOT_RULES_DENY_LOGON, "RET_FIX_SLOT_RULES_DENY_LOGON" },
        { RET_FIX_SLOT_RUNTIME_LIMIT_REACHED, "RET_FIX_SLOT_RUNTIME_LIMIT_REACHED" },
        { RET_FIX_RUNTIME_SESSION_UNAVAILABLE, "RET_FIX_RUNTIME_SESSION_UNAVAILABLE" },
        { RET_TRADE_HAS_OPEN, "RET_TRADE_HAS_OPEN" },

        { RET_FEED_ALREADY_LOAD, "RET_FEED_ALREADY_LOAD" },

        { RET_CFG_LAST_ADMIN, "RET_CFG_LAST_ADMIN" },
        { RET_CFG_LAST_ADMIN_GROUP, "RET_CFG_LAST_ADMIN_GROUP" },
        { RET_CFG_NOT_EMPTY, "RET_CFG_NOT_EMPTY" },
        { RET_CFG_INVALID_RANGE, "RET_CFG_INVALID_RANGE" },
        { RET_CFG_NOT_MANAGER_LOGIN, "RET_CFG_NOT_MANAGER_LOGIN" },
        { RET_CFG_BUILTIN, "RET_CFG_BUILTIN" },
        { RET_CFG_DUPLICATE, "RET_CFG_DUPLICATE" },
        { RET_CFG_LIMIT_REACHED, "RET_CFG_LIMIT_REACHED" },
        { RET_CFG_NO_ACCESS_TO_MAIN, "RET_CFG_NO_ACCESS_TO_MAIN" },
        { RET_CFG_DEALER_ID_EXIST, "RET_CFG_DEALER_ID_EXIST" },
        { RET_CFG_BIND_ADDR_EXIST, "RET_CFG_BIND_ADDR_EXIST" },
        { RET_CFG_WORKING_TRADE, "RET_CFG_WORKING_TRADE" },
        { RET_CFG_GATEWAY_NAME_EXIST, "RET_CFG_GATEWAY_NAME_EXIST" },
        { RET_CFG_SWITCH_TO_BACKUP, "RET_CFG_SWITCH_TO_BACKUP" },
        { RET_CFG_NO_BACKUP_MODULE, "RET_CFG_NO_BACKUP_MODULE" },
        { RET_CFG_NO_TRADE_MODULE, "RET_CFG_NO_TRADE_MODULE" },
        { RET_CFG_NO_HISTORY_MODULE, "RET_CFG_NO_HISTORY_MODULE" },
        { RET_CFG_ANOTHER_SWITCH, "RET_CFG_ANOTHER_SWITCH" },
        { RET_CFG_NO_LICENSE_FILE, "RET_CFG_NO_LICENSE_FILE" },
        { RET_CFG_GATEWAY_LOGIN_EXIST, "RET_CFG_GATEWAY_LOGIN_EXIST" },
        { RET_LICENSE_NOT_ACTIVE, "RET_LICENSE_NOT_ACTIVE" },

        { RET_BAD_REQUEST, "RET_BAD_REQUEST" },
        { RET_UNAUTHORIZED, "RET_UNAUTHORIZED" },
        { RET_FIELD_REQUIRED, "RET_FIELD_REQUIRED" },
        { RET_FORBIDDEN, "RET_FORBIDDEN" },
        { RET_NOT_FOUND, "RET_NOT_FOUND" },
        { RET_METHOD_NOT_ALLOWED, "RET_METHOD_NOT_ALLOWED" },
        { RET_INVALID_METHOD, "RET_INVALID_METHOD" },
        { RET_TIMEOUT, "RET_TIMEOUT" }
    };

    for (const auto& error : errors)
        if (error.code == retcode)
            return error.desc;

    return "RET_UNKNOWN";
}

struct WebErrorInfo {
    int http_status = 500;
    std::string error;
    std::string message;
};

inline int FormatWebStatus(const int retcode) {
    switch (retcode) {
        case RET_OK:
            return 200;
        case RET_INVALID_DATA:
        case RET_ERR_PARAMS:
        case RET_ERR_DATA:
        case RET_TRADE_BAD_PRICES:
        case RET_TRADE_BAD_STOPS:
        case RET_TRADE_BAD_VOLUME:
        case RET_TRADE_INVALID_SL_TP:
        case RET_TRADE_BAD_CLOSE_TIME:
        case RET_TRADE_INCORRECT_STATE:
        case RET_TRADE_INCORRECT_CMD:
        case RET_TRADE_INCORRECT_ORDER:
            return 400;
        case RET_LICENSE_NOT_ACTIVE:
        case RET_TRADE_DISABLE:
        case RET_TRADE_READ_ONLY:
        case RET_ACCOUNT_DISABLED:
        case RET_NOT_ENOUGH_RIGHTS:
        case RET_ERR_PERMISSIONS:
            return 403;
        case RET_USER_NOT_FOUND:
        case RET_GROUP_NOT_FOUND:
        case RET_SEC_NOT_FOUND:
        case RET_SYMBOL_NOT_FOUND:
        case RET_TRADE_SEC_NOT_FOUND:
        case RET_NOT_FOUND:
            return 404;
        case RET_TRADE_NO_MONEY:
        case RET_SYMBOL_USE_BY_TRADE:
        case RET_USR_HAS_TRADES:
        case RET_TRADE_HEDGE_PROHIBITED:
        case RET_TRADE_LIMIT_REACHED:
        case RET_TRADE_EXIST:
        case RET_TRADE_HAS_OPEN:
        case RET_DUPLICATE_RECORD:
        case RET_CFG_DUPLICATE:
            return 409;
        case RET_TRADE_MARKET_CLOSED:
            return 423;
        case RET_TOO_FREQUENT:
        case RET_ERR_FREQUENT:
        case RET_TRADE_TOO_MANY_REQ:
            return 429;
        default:
            return 500;
    }
}

inline int FormatWebCode(const int retcode) {
    return FormatWebStatus(retcode);
}

inline WebErrorInfo FormatWebError(const int retcode) {
    return WebErrorInfo{
        FormatWebStatus(retcode),
        FormatErrorCode(retcode),
        FormatError(retcode)
    };
}
