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

        // Write formatted header to retrieve infos later
        std::string newHeader = formatHeader(columnNames, rowCount, columnTypes);
        std::ofstream headerFile("header_info.txt");

        if (!headerFile.is_open()) {
            throw std::runtime_error("Unable to create header_info.txt file");
        }

        headerFile << newHeader << "\n";
        headerFile.close();

        outputFile.close();

        std::ifstream oldOutputFile(outputFilename);
        std::ofstream finalOutput("merged.csv");
        finalOutput << newHeader << "\n";
        
        // Append the rest of the original output file
        std::string line;
        while (std::getline(oldOutputFile, line)) {
            finalOutput << line << "\n";
        }

        // Close files
        oldOutputFile.close();
        finalOutput.close();

                // Replace the original output file with the updated file
        std::remove(outputFilename.c_str());
        std::rename("merged.csv", outputFilename.c_str());

       
    }

    std::unordered_map<std::string, std::string>* getColumnTypes() {
        return &columnTypes;
    }

};

#endif // PARSER_HPP

