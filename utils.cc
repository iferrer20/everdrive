#include "utils.h"

bool validDrivename(const std::string& s) {
    std::regex r {"^[a-zA-Z0-9_]{1,20}$"};
    return std::regex_match(s, r);
}

void trimstr(std::string& str, char trimchar) {
    for (std::size_t i{1}; i<str.size(); i++) { // Trim
        if (str[i] == '/' && str[i-1] == '/') {
            str.erase(i-1, 1);
            i--;
        }
    }
}
