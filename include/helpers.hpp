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

namespace Toolbox {
    namespace dfw{ 
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
                std::regex(R"(^\d{4}-\d{2}-\d{2}$)"),         // ISO 8601 (e.g., 2024-11-29)
                std::regex(R"(^\d{2}/\d{2}/\d{4}$)"),         // AME (e.g., 11/29/2024)
                std::regex(R"(^\d{2}-\d{2}-\d{4}$)"),         // Alternative AME (e.g., 11-29-2024)
                std::regex(R"(^\d{4}/\d{2}/\d{2}$)"),         // Alternative ISO (e.g., 2024/11/29)
                std::regex(R"(^\d{2}\s[a-zA-Z]+\s\d{4}$)"),   // Day Month Year (e.g., 29 November 2024)
                std::regex(R"(^[a-zA-Z]+\s\d{2},\s\d{4}$)")   // Month Day, Year (e.g., November 29, 2024)
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

            bool isDateTimeISO(const std::string& str) {
            std::regex isoPattern(R"(^\d{4}-\d{2}-\d{2}$)"); // Example: 2024-11-29
            return std::regex_match(str, isoPattern);
        }

        bool isDateTimeAME(const std::string& str) {
            std::regex amePattern(R"(^\d{2}/\d{2}/\d{4}$)"); // Example: 11/29/2024
            return std::regex_match(str, amePattern);
        }


        void updateColumnType(const std::string& currentColumn, const std::string& cell,
                std::vector<std::pair<std::string, std::string>>& columnTypes) {
            // Find the column entry by iterating through the vector
            auto it = std::find_if(columnTypes.begin(), columnTypes.end(),
                    [&currentColumn](const std::pair<std::string, std::string>& p) {
                    return p.first == currentColumn;
                    });

            // If the column type is not found, add a new entry with "Unknown" type
            if (it == columnTypes.end()) {
                columnTypes.push_back({currentColumn, "Unknown"});
                it = std::prev(columnTypes.end());
            }

            // Now, `it` points to the correct column pair
            if (it->second == "Unknown") {
                if (isInteger(cell)) {
                    it->second = "int";
                } else if (isDouble(cell)) {
                    it->second = "double";
                } else if (isDate(cell)) {
                    if (isDateTimeISO(cell)) {
                        it->second = "DateTimeISO";
                    } else if (isDateTimeAME(cell)) {
                        it->second = "DateTimeAME";
                    } else {
                        it->second = "DateTimeISO";
                    }
                } else {
                    it->second = "string";
                }
            } else if (it->second == "integer" && !isInteger(cell)) {
                it->second = isDouble(cell) ? "double" : "string";
            } else if (it->second == "double" && !isDouble(cell)) {
                it->second = "string";
            } else if (it->second == "date" && !isDate(cell)) {
                it->second = "string";
            } else if (it->second == "DateTimeISO" && !isDateTimeISO(cell)) {
                it->second = "string";
            } else if (it->second == "DateTimeAME" && !isDateTimeAME(cell)) {
                it->second = "string";
            }
        }

        std::string formatHeader(const std::vector<std::pair<std::string, std::string>>& columnNames, size_t rowCount) {
            std::ostringstream formattedHeader;
            for (size_t i = 0; i < columnNames.size(); ++i) {
                if (i > 0) {
                    formattedHeader << ",";
                }
                // Access column name and type directly from the pair in columnNames
                formattedHeader << columnNames[i].first << ":" << rowCount << ":<" << columnNames[i].second << ">";
            }
            return formattedHeader.str();
        }

        template <typename T1, typename T2>
            std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& pair) {
                os << pair.first << ": " << pair.second;
                return os;
            }

        // Function to check if the header is correctly formatted
        bool isFormattedHeader(const std::string& header) {
            // Define the regular expression for a correctly formatted column
            const std::regex column_format(R"((\w+):(\d+):<(\w+)>)");

            // Split the header into individual columns using ',' as a delimiter
            std::istringstream header_stream(header);
            std::string column;
            while (std::getline(header_stream, column, ',')) {
                // Check if the column matches the format
                if (!std::regex_match(column, column_format)) {
                    return false; // If any column doesn't match, the header is invalid
                }
            }

            return true; // All columns are valid
        }

    }
}
#endif // HELPERS_HPP

