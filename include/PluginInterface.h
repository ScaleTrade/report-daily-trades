#pragma once

#include <vector>
#include <sstream>
#include <thread>
#include <atomic>
#include <string>
#include "ReportServerInterface.h"
#include <rapidjson/document.h>
#include "ast/Ast.hpp"
#include "sbxTableBuilder/SBXTableBuilder.hpp"
#include "utils/Utils.h"
#include "structures/ReportStructures.h"
#include "structures/ReportType.h"

using namespace ast;

extern "C" {
    void AboutReport(rapidjson::Value& request,
                     rapidjson::Value& response,
                     rapidjson::Document::AllocatorType& allocator,
                     ReportServerInterface* server);

    void DestroyReport();

    void CreateReport(rapidjson::Value& request,
                     rapidjson::Value& response,
                     rapidjson::Document::AllocatorType& allocator,
                     ReportServerInterface* server);
}