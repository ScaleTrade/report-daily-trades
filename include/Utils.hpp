#pragma once

#include "ast/Ast.hpp"
#include "Structures.hpp"

namespace utils {
    void CreateUI(const ast::Node& node,
              rapidjson::Value& response,
              rapidjson::Document::AllocatorType& allocator);
}