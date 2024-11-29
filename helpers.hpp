#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <regex>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>

// Helper functions to determine data types
bool isInteger(const std::string& str) {
    if (str.empty()) return false;
    size_t i = (str[0] == '-' || str[0] == '+') ? 1 : 0;
    for (; i < str.size(); ++i) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

bool isDouble(const std::string& str) {
    if (str.empty()) return false;
    bool decimalPoint = false;
    bool digitFound = false;
    size_t i = (str[0] == '-' || str[0] == '+') ? 1 : 0;
    for (; i < str.size(); ++i) {
        if (str[i] == '.') {
            if (decimalPoint) return false;
            decimalPoint = true;
        } else if (isdigit(str[i])) {
            digitFound = true;
        } else {
            return false;
        }
    }
    return digitFound;
}

bool isDate(const std::string& str) {
    if (str.empty()) return false;
    std::regex datePatterns[] = {
        std::regex(R"(^\d{4}-\d{2}-\d{2}$)"),
        std::regex(R"(^\d{2}/\d{2}/\d{4}$)"),
        std::regex(R"(^\d{2}-\d{2}-\d{4}$)"),
        std::regex(R"(^\d{4}/\d{2}/\d{2}$)"),
        std::regex(R"(^\d{2}\s[a-zA-Z]+\s\d{4}$)"),
        std::regex(R"(^[a-zA-Z]+\s\d{2},\s\d{4}$)")
    };
    for (const auto& pattern : datePatterns) {
        if (std::regex_match(str, pattern)) {
            return true;
        }
    }
    return false;
}

std::string cleanString(const std::string& str) {
    std::string cleaned = str;
    cleaned.erase(
        std::remove_if(cleaned.begin(), cleaned.end(),
                       [](unsigned char c) { return !std::isprint(c) || std::iscntrl(c) || std::isspace(c); }),
        cleaned.end());
    return cleaned;
}

void updateColumnType(const std::string& currentColumn, const std::string& cell,
                      std::unordered_map<std::string, std::string>& columnTypes) {
    if (columnTypes[currentColumn] == "Unknown") {
        if (isInteger(cell)) {
            columnTypes[currentColumn] = "int";
        } else if (isDouble(cell)) {
            columnTypes[currentColumn] = "double";
        } else if (isDate(cell)) {
            columnTypes[currentColumn] = "date";
        } else {
            columnTypes[currentColumn] = "string";
        }
    } else if (columnTypes[currentColumn] == "integer" && !isInteger(cell)) {
        columnTypes[currentColumn] = isDouble(cell) ? "double" : "string";
    } else if (columnTypes[currentColumn] == "double" && !isDouble(cell)) {
        columnTypes[currentColumn] = "string";
    } else if (columnTypes[currentColumn] == "date" && !isDate(cell)) {
        columnTypes[currentColumn] = "string";
    }
}

std::string formatHeader(const std::vector<std::string>& columnNames, size_t rowCount,
                         const std::unordered_map<std::string, std::string>& columnTypes) {
    std::ostringstream formattedHeader;
    for (size_t i = 0; i < columnNames.size(); ++i) {
        if (i > 0) {
            formattedHeader << ",";
        }
        formattedHeader << columnNames[i] << ":" << rowCount << ":<" << columnTypes.at(columnNames[i]) << ">";
    }
    return formattedHeader.str();
}

#endif // HELPERS_HPP

