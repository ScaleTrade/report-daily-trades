#pragma once

#include <rapidjson/document.h>
#include <sstream>
#include <thread>
#include <atomic>
#include <string>
#include "Structures.hpp"
#include "ast/Ast.hpp"
#include "Utils.hpp"

extern "C" {
    void AboutReport(rapidjson::Value& request,
                     rapidjson::Value& response,
                     rapidjson::Document::AllocatorType& allocator,
                     CServerInterface* server);

    void DestroyReport();

    void CreateReport(rapidjson::Value& request,
                     rapidjson::Value& response,
                     rapidjson::Document::AllocatorType& allocator,
                     CServerInterface* server);
}