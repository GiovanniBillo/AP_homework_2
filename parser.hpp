#ifndef PARSER_HPP
#define PARSER_HPP

#include "helpers.hpp"
#include <fstream>
#include <stdexcept>

class CSVParser {
private:
    std::vector<std::string> columnNames;
    std::unordered_map<std::string, std::string> columnTypes;

public:
    void parse(const std::string& inputFilename, const std::string& outputFilename) {
        std::ifstream file(inputFilename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open the input file!");
        }

        std::ofstream outputFile(outputFilename);
        if (!outputFile.is_open()) {
            throw std::runtime_error("Could not open the output file!");
        }

        // Read and parse the header
        std::string headerLine;
        std::getline(file, headerLine);
        std::stringstream headerStream(headerLine);
        std::string columnName;
        while (std::getline(headerStream, columnName, ',')) {
            columnNames.push_back(cleanString(columnName));
        }
        columnNames[0] = "INDEX";

        // Initialize column types
        for (const auto& name : columnNames) {
            columnTypes[name] = "Unknown";
        }

        // Parse data rows
        size_t rowCount = 0;
        std::string rowLine;
        while (std::getline(file, rowLine)) {
            std::stringstream rowStream(rowLine);
            std::string cell;
            size_t colIndex = 0;
            while (std::getline(rowStream, cell, ',')) {
                if (colIndex >= columnNames.size()) break;
                updateColumnType(columnNames[colIndex], cleanString(cell), columnTypes);
                ++colIndex;
            }
            ++rowCount;
            outputFile << rowLine << "\n";
        }

        // Write formatted header
        std::string newHeader = formatHeader(columnNames, rowCount, columnTypes);
        outputFile.close();
        std::ofstream finalOutput(outputFilename);
        finalOutput << newHeader << "\n";
    }
};

#endif // PARSER_HPP

