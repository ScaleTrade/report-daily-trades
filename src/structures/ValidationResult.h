#pragma once

#include <string>

struct ValidationResult {
    bool        allowed = false;
    int         code    = 0;
    std::string message;
};