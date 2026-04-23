#pragma once

#include <ctime>
#include <rapidjson/document.h>
#include <string>
#include <vector>

#include "Structures.h"
#include "model/ReportAccount.hpp"
#include "model/ReportCandle.hpp"
#include "model/ReportGroup.hpp"
#include "model/ReportLog.hpp"
#include "model/ReportSymbol.hpp"
#include "model/ReportTrade.hpp"

class Core;

inline int REPORT_SERVER_API = 1;

class ReportServerInterface {
public:
    virtual ~ReportServerInterface() = default;

    static int GetApiVersion() { return REPORT_SERVER_API; }

    virtual int GetLogs(time_t from, time_t to, const std::string& type, const std::string& filter, std::vector<ReportServerLog>* logs) = 0;

    virtual int GetAccountsByGroup(const std::string& group, std::vector<ReportAccountRecord>* accounts) = 0;
    virtual int GetAccountByLogin(int login, ReportAccountRecord* account) = 0;
    virtual int GetAccountBalanceByLogin(int login, ReportMarginLevel* margin) = 0;
    virtual int GetMarginLevelByGroup(const std::string& group, std::vector<ReportMarginLevel>* margins) = 0;
    virtual int GetAccountsEquitiesByGroup(time_t from, time_t to, const std::string& group_filter, std::vector<ReportEquityRecord>* equities) = 0;
    virtual int GetAccountsEquitiesByLogin(time_t from, time_t to, int login, std::vector<ReportEquityRecord>* equities) = 0;

    virtual int GetOpenTradesByLogin(int login, std::vector<ReportTradeRecord>* trades) = 0;
    virtual int GetPendingTradesByLogin(int login, std::vector<ReportTradeRecord>* trades) = 0;
    virtual int GetOpenTradesByMagic(int magic, std::vector<ReportTradeRecord>* trades) = 0;
    virtual int GetOpenTradeByOrder(int order, ReportTradeRecord* trade) = 0;
    virtual int GetOpenTradeByGwUUID(const std::string& gw_uuid, ReportTradeRecord* trade) = 0;
    virtual int GetCloseTradeByGwUUID(const std::string& gw_uuid, ReportTradeRecord* trade) = 0;
    virtual int GetOpenTradeByGwOrder(int gw_order, ReportTradeRecord* trade) = 0;
    virtual int GetCloseTradeByGwOrder(int gw_order, ReportTradeRecord* trade) = 0;
    virtual int GetCloseTradesByLogin(int login, std::vector<ReportTradeRecord>* trades) = 0;
    virtual int GetCloseTradesByGroup(const std::string& filter_group, time_t from, time_t to, std::vector<ReportTradeRecord>* trades) = 0;
    virtual int GetPendingTradesByGroup(const std::string& filter_group, time_t from, time_t to, std::vector<ReportTradeRecord>* trades) = 0;
    virtual int GetOpenTradesByGroup(const std::string& filter_group, time_t from, time_t to, std::vector<ReportTradeRecord>* trades) = 0;
    virtual int GetAllOpenTrades(std::vector<ReportTradeRecord>* trades) = 0;
    virtual int GetTransactionsByGroup(const std::string& filter_group, time_t from, time_t to, std::vector<ReportTradeRecord>* trades) = 0;
    virtual int GetTransactionsByLogin(int login, time_t from, time_t to, std::vector<ReportTradeRecord>* trades) = 0;

    virtual int CalculateCommission(const ReportTradeRecord& trade, double* calculated_commission) = 0;
    virtual int CalculateSwap(const ReportTradeRecord& trade, double* calculated_swap) = 0;
    virtual int CalculateProfit(const ReportTradeRecord& trade, double* calculated_profit) = 0;
    virtual int CalculateMargin(const ReportTradeRecord& trade, double* calculated_margin) = 0;
    virtual int CalculateConvertRateByCurrency(const std::string& from_cur, const std::string& to_cur, int cmd, double* multiplier) = 0;

    virtual int GetSymbol(const std::string& symbol, ReportSymbolRecord* cs) = 0;
    virtual int GetGroup(const std::string& group_name, ReportGroupRecord* group) = 0;
    virtual int GetAllGroups(std::vector<ReportGroupRecord>* groups) = 0;

    virtual int GetCandles(const std::string& symbol, const std::string& frame, time_t from, time_t to, std::vector<ReportCandleRecord>* candles) = 0;
};
