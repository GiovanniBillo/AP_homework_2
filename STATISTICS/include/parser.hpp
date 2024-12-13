#ifndef PARSER_HPP
#define PARSER_HPP

#include "helpers.hpp"
#include <fstream>
#include <stdexcept>

namespace Toolbox{
    namespace dfw{ 
        class CSVParser {
            private:
                std::vector<std::pair<std::string, std::string>> columnNames;
            public:
                void parse(const std::string& inputFilename, const std::string& outputFilename) {
                    std::ifstream inputFile(inputFilename);
                    if (!inputFile.is_open()) {
                        throw std::runtime_error("Could not open the input file!");
                    }

                    std::ofstream outputFile(outputFilename);
                    if (!outputFile.is_open()) {
                        throw std::runtime_error("Could not open the output file!");
                    }
                    // Read and store the header as a single string
                    std::string header;
                    std::getline(inputFile, header);

                    // Optionally print the header to verify
                    std::cout << "Header: " << header << std::endl;

                    if (!isFormattedHeader(header))
                    { 
                        // Reset file reading to the beginning
                        inputFile.clear();               // Clear any error flags
                        inputFile.seekg(0, std::ios::beg); // Move to the beginning of the file
                                                           // Read and parse the header
                        std::string headerLine;
                        std::getline(inputFile, headerLine);
                        std::stringstream headerStream(headerLine);
                        std::string columnName;
                        while (std::getline(headerStream, columnName, ',')) {
                            columnNames.emplace_back(cleanString(columnName), "Unknown");
                        }
                        columnNames[0].first = "INDEX";

                        // Parse data rows
                        size_t rowCount = 0;
                        std::string rowLine;
                        while (std::getline(inputFile, rowLine)) {
                            std::stringstream rowStream(rowLine);
                            std::string cell;
                            size_t colIndex = 0;
                            while (std::getline(rowStream, cell, ',')) {
                                if (colIndex >= columnNames.size()) break;
                                updateColumnType(columnNames[colIndex].first, cleanString(cell), columnNames);
                                ++colIndex;
                            }
                            ++rowCount;
                            outputFile << rowLine << "\n";
                        }

                        /* // Write formatted header to retrieve infos later */
                        std::string newHeader = formatHeader(columnNames, rowCount);
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

                    }else
                    {               
                        inputFile.clear();               // Clear any error flags
                        inputFile.seekg(0, std::ios::beg); // Move to the beginning of the file
                        outputFile << inputFile.rdbuf(); // Copy file contents directly
                        inputFile.close(); 
                        outputFile.close(); 
                    }
                }
                std::vector<std::pair<std::string, std::string>> getColumnTypes() {
                    return columnNames;
                }

        };

    }
}
#endif // PARSER_HPP

