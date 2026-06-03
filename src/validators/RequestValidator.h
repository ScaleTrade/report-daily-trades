#pragma once

#include <iostream>
#include <string>

#include "ReportServerInterface.h"
#include "rapidjson/document.h"
#include "structures/ReportType.h"
#include "structures/ValidationResult.h"
#include "utils/Utils.h"

class RequestValidator {
public:
    static ValidationResult ValidateRequest(ReportType              report_type,
                                            const rapidjson::Value& request,
                                            ReportServerInterface*  server);

private:
    static ValidationResult ValidateNone(const rapidjson::Value& request,
                                         ReportServerInterface*  server);

    static ValidationResult ValidateRange(const rapidjson::Value& request,
                                          ReportServerInterface*  server);

    static ValidationResult ValidateDaily(const rapidjson::Value& request,
                                          ReportServerInterface*  server);

    static ValidationResult ValidateAccount(const rapidjson::Value& request,
                                            ReportServerInterface*  server);

    static ValidationResult ValidateSymbol(const rapidjson::Value& request,
                                           ReportServerInterface*  server);

    static ValidationResult ValidateGroup(const rapidjson::Value& request,
                                          ReportServerInterface*  server);

    static ValidationResult ValidateRangeGroup(const rapidjson::Value& request,
                                               ReportServerInterface*  server);

    static ValidationResult ValidateDailyGroup(const rapidjson::Value& request,
                                               ReportServerInterface*  server);

    static ValidationResult ValidateRangeAccount(const rapidjson::Value& request,
                                                 ReportServerInterface*  server);

    static ValidationResult ValidateDailyAccount(const rapidjson::Value& request,
                                                 ReportServerInterface*  server);

    static ValidationResult ValidateRangeSymbol(const rapidjson::Value& request,
                                                ReportServerInterface*  server);

    static ValidationResult ValidateDailySymbol(const rapidjson::Value& request,
                                                ReportServerInterface*  server);

    static ValidationResult ValidateRangeGroupSymbol(const rapidjson::Value& request,
                                                     ReportServerInterface*  server);

    static ValidationResult ValidateDailyGroupSymbol(const rapidjson::Value& request,
                                                     ReportServerInterface*  server);
};