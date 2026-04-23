#pragma once

#include <string>

struct ReportServerLog {
    std::string time;
    std::string actor_type;
    std::string actor_id;
    std::string action;
    std::string status;
    std::string source;
    std::string detail;
};
