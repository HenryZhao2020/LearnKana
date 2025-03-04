#include "strutil.h"

std::string strutil::strip(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) {
        return "";
    }

    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

std::string strutil::tolower(const std::string& str) {
    std::string lower = str;
    for (auto& c : lower) {
        c = std::tolower(c);
    }
    return lower;
}
