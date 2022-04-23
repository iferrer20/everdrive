#include "utils.h"

bool validDrivename(const std::string& s) {
    std::regex r {"^[a-zA-Z0-9_]{1,20}$"};
    return std::regex_match(s, r);
}