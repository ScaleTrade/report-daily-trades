#pragma once

enum class ReportType {
    None,             // report without filter
    Range,            // report filter with time range
    Daily,            // report filter by one day
    Account,          // report filter by account
    Symbol,           // report filter by account
    Group,            // report filter with group mask
    RangeGroup,       // report filter one day and group mask
    DailyGroup,       // report filter one day and group mask
    RangeAccount,     // report filter with time range and account
    DailyAccount,     // report filter by one day and account
    RangeSymbol,      // report filter with time range and account
    DailySymbol,      // report filter by one day and account
    RangeGroupSymbol, // report filter with time range and group and symbols
    DailyGroupSymbol  // report filter by one day and groups and symbols
};