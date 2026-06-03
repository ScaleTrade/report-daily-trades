#include "RequestValidator.h"

ValidationResult RequestValidator::ValidateRequest(ReportType              report_type,
                                                   const rapidjson::Value& request,
                                                   ReportServerInterface*  server) {

    switch (report_type) {
        case ReportType::None:
            return ValidateNone(request, server);

        case ReportType::Range:
            return ValidateRange(request, server);

        case ReportType::Daily:
            return ValidateDaily(request, server);

        case ReportType::Account:
            return ValidateAccount(request, server);

        case ReportType::Symbol:
            return ValidateSymbol(request, server);

        case ReportType::Group:
            return ValidateGroup(request, server);

        case ReportType::RangeGroup:
            return ValidateRangeGroup(request, server);

        case ReportType::DailyGroup:
            return ValidateDailyGroup(request, server);

        case ReportType::RangeAccount:
            return ValidateRangeAccount(request, server);

        case ReportType::DailyAccount:
            return ValidateDailyAccount(request, server);

        case ReportType::RangeSymbol:
            return ValidateRangeSymbol(request, server);

        case ReportType::DailySymbol:
            return ValidateDailySymbol(request, server);

        case ReportType::RangeGroupSymbol:
            return ValidateRangeGroupSymbol(request, server);

        case ReportType::DailyGroupSymbol:
            return ValidateDailyGroupSymbol(request, server);

        default: {
            ValidationResult result;
            result.allowed = false;
            result.code    = 404;
            result.message = "Unknown report type";
            return result;
        }
    }
}

ValidationResult RequestValidator::ValidateNone(const rapidjson::Value& request,
                                                ReportServerInterface*  server) {
    ValidationResult result;
    result.allowed = true;
    result.code    = 200;
    result.message = "None: access granted (stub)";
    return result;
}

ValidationResult RequestValidator::ValidateRange(const rapidjson::Value& request,
                                                 ReportServerInterface*  server) {
    ValidationResult result;
    result.allowed = true;
    result.code    = 200;
    result.message = "Range: access granted (stub)";
    return result;
}

ValidationResult RequestValidator::ValidateDailyGroup(const rapidjson::Value& request,
                                                      ReportServerInterface*  server) {
    ValidationResult result;

    if (!request.HasMember("group") || !request["group"].IsString()) {
        result.allowed = false;
        result.code    = 400;
        result.message = "ValidateDailyGroup: missing or invalid 'group'";
        return result;
    }

    if (!request.HasMember("from") || !request["from"].IsNumber()) {
        result.allowed = false;
        result.code    = 400;
        result.message = "ValidateDailyGroup: missing or invalid 'from'";
        return result;
    }

    if (!request.HasMember("to") || !request["to"].IsNumber()) {
        result.allowed = false;
        result.code    = 400;
        result.message = "ValidateDailyGroup: missing or invalid 'to'";
        return result;
    }

    result.allowed = true;
    result.code    = 200;
    result.message = "ValidateDailyGroup: access granted";
    return result;
}

ValidationResult RequestValidator::ValidateAccount(const rapidjson::Value& request,
                                                   ReportServerInterface*  server) {
    ValidationResult result;
    result.allowed = true;
    result.code    = 200;
    result.message = "Account: access granted (stub)";
    return result;
}

ValidationResult RequestValidator::ValidateSymbol(const rapidjson::Value& request,
                                                  ReportServerInterface*  server) {
    ValidationResult result;
    result.allowed = true;
    result.code    = 200;
    result.message = "Symbol: access granted (stub)";
    return result;
}

ValidationResult RequestValidator::ValidateRangeGroup(const rapidjson::Value& request,
                                                      ReportServerInterface*  server) {
    ValidationResult result;

    if (!request.HasMember("group") || !request["group"].IsString()) {
        result.allowed = false;
        result.code    = 400;
        result.message = "ValidateRangeGroup: missing or invalid 'group'";
        return result;
    }

    if (!request.HasMember("from") || !request["from"].IsNumber()) {
        result.allowed = false;
        result.code    = 400;
        result.message = "ValidateRangeGroup: missing or invalid 'from'";
        return result;
    }

    if (!request.HasMember("to") || !request["to"].IsNumber()) {
        result.allowed = false;
        result.code    = 400;
        result.message = "ValidateRangeGroup: missing or invalid 'to'";
        return result;
    }

    const rapidjson::Value& access        = request["__access"];
    const std::string       access_groups = access["groups"].GetString();

    int match_result = 0;
    try {
        match_result = server->MatchWildCardGroup(access_groups, request["group"].GetString());
    } catch (const std::exception& e) {
        result.allowed = false;
        result.code    = 404;
        result.message = "ValidateRangeAccount: MatchWildCardGroup error";
        return result;
    }

    std::cout << "MatchWildCardGroup: " << match_result << std::endl;

    if (match_result != 0) {
        result.allowed = false;
        result.code    = 403;
        result.message = "ValidateRangeGroup: access denied (group does not match required mask)";
        return result;
    }

    result.allowed = true;
    result.code    = 200;
    result.message = "ValidateRangeGroup: access granted";
    return result;
}

ValidationResult RequestValidator::ValidateGroup(const rapidjson::Value& request,
                                                 ReportServerInterface*  server) {
    ValidationResult result;

    if (!request.HasMember("group") || !request["group"].IsString()) {
        result.allowed = false;
        result.code    = 400;
        result.message = "ValidateGroup: missing or invalid 'group'";
        return result;
    }

    result.allowed = true;
    result.code    = 200;
    result.message = "ValidateGroup: access granted";
    return result;
}

ValidationResult RequestValidator::ValidateDaily(const rapidjson::Value& request,
                                                 ReportServerInterface*  server) {
    ValidationResult result;

    if (!request.HasMember("from") || !request["from"].IsNumber()) {
        result.allowed = false;
        result.code    = 400;
        result.message = "ValidateDaily: missing or invalid 'from'";
        return result;
    }

    if (!request.HasMember("to") || !request["to"].IsNumber()) {
        result.allowed = false;
        result.code    = 400;
        result.message = "ValidateDaily: missing or invalid 'to'";
        return result;
    }

    result.allowed = true;
    result.code    = 200;
    result.message = "ValidateDaily: access granted";
    return result;
}

ValidationResult RequestValidator::ValidateRangeAccount(const rapidjson::Value& request,
                                                        ReportServerInterface*  server) {
    ValidationResult result;

    if (!request.HasMember("login") || !request["login"].IsNumber()) {
        result.allowed = false;
        result.code    = 400;
        result.message = "ValidateRangeAccount: missing or invalid 'login'";
        return result;
    }

    if (!request.HasMember("from") || !request["from"].IsNumber()) {
        result.allowed = false;
        result.code    = 400;
        result.message = "ValidateRangeAccount: missing or invalid 'from'";
        return result;
    }

    if (!request.HasMember("to") || !request["to"].IsNumber()) {
        result.allowed = false;
        result.code    = 400;
        result.message = "ValidateRangeAccount: missing or invalid 'to'";
        return result;
    }

    const rapidjson::Value& access = request["__access"];
    const std::string       groups = access["groups"].GetString();
    if (groups == "*") {
        result.allowed = true;
        result.code    = 200;
        result.message = "ValidateRangeAccount: access granted (user has all groups)";
        return result;
    }

    ReportAccountRecord account_record{};

    try {
        server->GetAccountByLogin(request["login"].GetInt(), &account_record);
    } catch (const std::exception& e) {
        result.allowed = false;
        result.code    = 404;
        result.message = "ValidateRangeAccount: GetAccountByLogin error";
        return result;
    }

    const std::set<std::string> allowed_groups = utils::SplitToSet(groups);

    if (allowed_groups.find(account_record.group) == allowed_groups.end()) {
        result.allowed = false;
        result.code    = 403;
        result.message = "ValidateRangeAccount: access denied for group '" +
                         std::string(account_record.group) + "'";
        return result;
    }

    result.allowed = true;
    result.code    = 200;
    result.message = "ValidateRangeAccount: access granted";
    return result;
}

ValidationResult RequestValidator::ValidateDailyAccount(const rapidjson::Value& request,
                                                        ReportServerInterface*  server) {
    ValidationResult result;
    result.allowed = true;
    result.code    = 200;
    result.message = "DailyAccount: access granted (stub)";
    return result;
}

ValidationResult RequestValidator::ValidateRangeSymbol(const rapidjson::Value& request,
                                                       ReportServerInterface*  server) {
    ValidationResult result;
    result.allowed = true;
    result.code    = 200;
    result.message = "RangeSymbol: access granted (stub)";
    return result;
}

ValidationResult RequestValidator::ValidateDailySymbol(const rapidjson::Value& request,
                                                       ReportServerInterface*  server) {
    ValidationResult result;
    result.allowed = true;
    result.code    = 200;
    result.message = "DailySymbol: access granted (stub)";
    return result;
}

ValidationResult RequestValidator::ValidateRangeGroupSymbol(const rapidjson::Value& request,
                                                            ReportServerInterface*  server) {
    ValidationResult result;
    result.allowed = true;
    result.code    = 200;
    result.message = "RangeGroupSymbol: access granted (stub)";
    return result;
}

ValidationResult RequestValidator::ValidateDailyGroupSymbol(const rapidjson::Value& request,
                                                            ReportServerInterface*  server) {
    ValidationResult result;
    result.allowed = true;
    result.code    = 200;
    result.message = "DailyGroupSymbol: access granted (stub)";
    return result;
}