#pragma once

enum class ReportTradeCommand : int {
    Nothing = -1,
    Buy = 0,
    Sell = 1,
    BuyLimit = 2,
    SellLimit = 3,
    BuyStop = 4,
    SellStop = 5,
    BalanceIn = 6,
    CreditIn = 7,
    BalanceOut = 8,
    CreditOut = 9,
    BuyStopLimit = 10,
    SellStopLimit = 11,
    Charge = 12,
    Correction = 13,
    Bonus = 14,
    Commission = 15,
    CommissionDayly = 16,
    CommissionMonthly = 17,
    InterestRate = 18,
    Dividend = 19,
    DividendFranked = 20,
    Tax = 21,
    SoCompensation = 22
};

enum class ReportTradeState : int {
    Rollover = -2,
    NoChange = -1,
    OpenNormal = 0,
    OpenRequest = 1,
    OpenRestored = 2,
    ClosedNormal = 3,
    ClosedPart = 4,
    ClosedBy = 5,
    Deleted = 6,
    CloseRequest = 7,
    DeleteRequest = 8,
    CancelRequest = 9,
    FinanceOpenRequest = 10,
    FinanceUpdateRequest = 11,
    FinanceDeleteRequest = 12,
    RestoreRequest = 13,
    OpenUpdateRequest = 14,
    ClosedUpdateRequest = 15,
    PendingUpdateRequest = 16
};

enum class ReportTradeReason : int {
    Client = 0,
    Expert = 1,
    Dealer = 2,
    Signal = 3,
    Gateway = 4,
    Mobile = 5,
    Web = 6,
    Api = 7
};

enum class ReportTradeActivation : int {
    None = 0,
    StopLoss = 1,
    TakeProfit = 2,
    Pending = 3,
    StopOut = 4,
    Cancel = 5,
    StopLossRollback = -1,
    TakeProfitRollback = -2,
    PendingRollback = -3,
    StopOutRollback = -4,
    CancelRollback = -5
};
