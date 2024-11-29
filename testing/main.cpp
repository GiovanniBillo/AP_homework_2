#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <cctype>
#include <utility>
#include <algorithm>
#include <cctype> // For std::isprint
#include <regex>
/* #include "csv.h" // Include the csv-parser library header */
                 
// Helper functions to determine data types
bool isInteger(const std::string& str) {
    if (str.empty()) return false;
    size_t i = (str[0] == '-' || str[0] == '+') ? 1 : 0;
    for (; i < str.size(); ++i) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

/* bool isFloat(const std::string& str) { */
/*     if (str.empty()) return false; */
/*     bool decimalPoint = false; */
/*     size_t i = (str[0] == '-' || str[0] == '+') ? 1 : 0; */
/*     for (; i < str.size(); ++i) { */
/*         if (str[i] == '.') { */
/*             if (decimalPoint) return false; */
/*             decimalPoint = true; */
/*         } else if (!isdigit(str[i])) { */
/*             return false; */
/*         } */
/*     } */
/*     return decimalPoint; */
/* } */

bool isDouble(const std::string& str) {
    if (str.empty()) return false;

    bool decimalPoint = false;  // Track the presence of a decimal point
    bool digitFound = false;    // Ensure at least one digit is present
    size_t i = 0;

    // Check for optional sign at the beginning
    if (str[0] == '-' || str[0] == '+') {
        i = 1;
    }

    for (; i < str.size(); ++i) {
        if (str[i] == '.') {
            if (decimalPoint) return false;  // More than one decimal point
            decimalPoint = true;             // Mark the first decimal point
        } else if (isdigit(str[i])) {
            digitFound = true;               // Mark that a digit is found
        } else {
            return false;                    // Invalid character found
        }
    }

    // A valid double must have at least one digit and can optionally have a decimal point
    return digitFound;
}

bool isDate(const std::string& str) {
    if (str.empty()) return false;

    // Common date patterns
    std::regex datePatterns[] = {
        std::regex(R"(^\d{4}-\d{2}-\d{2}$)"),      // YYYY-MM-DD
        std::regex(R"(^\d{2}/\d{2}/\d{4}$)"),      // DD/MM/YYYY or MM/DD/YYYY
        std::regex(R"(^\d{2}-\d{2}-\d{4}$)"),      // DD-MM-YYYY or MM-DD-YYYY
        std::regex(R"(^\d{4}/\d{2}/\d{2}$)"),      // YYYY/MM/DD
        std::regex(R"(^\d{2}\s[a-zA-Z]+\s\d{4}$)"), // DD Month YYYY (e.g., 01 January 2023)
        std::regex(R"(^[a-zA-Z]+\s\d{2},\s\d{4}$)") // Month DD, YYYY (e.g., January 01, 2023)
    };

    // Check against all patterns
    for (const auto& pattern : datePatterns) {
        if (std::regex_match(str, pattern)) {
            return true;
        }
    }

    return false;
}

template <typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& pair) {
    os << pair.first << ": " << pair.second;
    return os;
}

// Helper function to remove non-printable characters and whitespace
std::string cleanString(const std::string& str) {
    std::string cleaned = str;
    cleaned.erase(
        std::remove_if(cleaned.begin(), cleaned.end(), [](unsigned char c) {
            return !std::isprint(c) || std::iscntrl(c) || std::isspace(c); // Remove non-printable, control, and whitespace characters
        }),
        cleaned.end());
    return cleaned;
}

void updateColumnType(const std::string& currentColumn,
                      const std::string& cell,
                      std::unordered_map<std::string, std::string>& columnTypes) {
    std::cout << "Entered function " << std::endl;
    std::cout << currentColumn << std::endl;
    std::cout << cell << std::endl;
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
        columnTypes[currentColumn] = isDouble(cell) ? "double" : (isDate(cell) ? "date" : "string");
    } else if (columnTypes[currentColumn] == "double" && !isDouble(cell)) {
        columnTypes[currentColumn] = isDate(cell) ? "date" : "string";
    } else if (columnTypes[currentColumn] == "date" && !isDate(cell)) {
        columnTypes[currentColumn] = "string";
    }

    return;
}

// Function to format the header
std::string formatHeader(const std::vector<std::string>& columnNames,
                         size_t rowCount,
                         const std::unordered_map<std::string, std::string>& columnTypes) {


    std::ostringstream formattedHeader;
    for (size_t i = 0; i < columnNames.size(); ++i) {
        if (i > 0) {
            formattedHeader << ","; // Add a comma between columns
        }
        formattedHeader << columnNames[i] << ":" << rowCount << ":<" << columnTypes.at(columnNames[i]) << ">";
    }
    return formattedHeader.str();
}

int main() {
    std::string filename;
    std::vector<std::string> columnNames;
    std::string outputFilename = "superstore_updated.csv";

  try {
        // Path to your CSV file
        filename = "superstore.csv";

        std::ofstream outputFile(outputFilename);
        if (!outputFile.is_open()) {
            throw std::runtime_error("Could not open the output file for writing!");
        }


        // Open the file
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open the file!");
        }

        // Read the first row (header)
        std::string headerLine;
        std::getline(file, headerLine); // Get the first line

        // Parse the header line
        std::stringstream headerStream(headerLine);
        std::string columnName;
        while (std::getline(headerStream, columnName, ',')) {
            columnName = cleanString(columnName);
            columnNames.push_back(columnName);
        }
        try {
        if (columnNames.empty()) {
            throw std::runtime_error("Error: The columnNames vector is empty. Cannot rename the first column to 'INDEX'.");
        }
        columnNames.at(0) = "INDEX";
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            throw; // Rethrow the exception if necessary
        }

        /* // Count the number of rows */
        /* std::string rowLine; */
        /* size_t rowCount = 0; */
        /* while (std::getline(file, rowLine)) { */
        /*     ++rowCount; */
        /* } */

        // Initialize column types
        size_t numColumns = columnNames.size();
        std::unordered_map<std::string, std::string> columnTypes;
        for (const auto& name : columnNames) {
            columnTypes[name] = "Unknown";
        }

        // Infer column types from the data
        /* size_t maxRows = 1000; // Limit to first 1000 rows */
        size_t rowCount = 0;
        std::string rowLine;
        
        // need to clear the EOF flag before looping over the file again to determine types 
        /* file.clear(); // Clear EOF flag */
        /* file.seekg(0, std::ios::beg); // Go back to the beginning of the file */

        while (std::getline(file, rowLine)) {
            std::stringstream rowStream(rowLine);
            std::string cell;
            size_t colIndex = 0;

            
            while (std::getline(rowStream, cell, ',')) {
                if (colIndex >= numColumns) break;
                const auto& currentColumn = columnNames[colIndex];
                // TODO: HANDLE INVISIBLE STRING CHARACTERS-> REMOVE FROM CELL
                cell = cleanString(cell);

                updateColumnType(currentColumn, cell, columnTypes); 

                /* if (columnTypes[currentColumn] == "Unknown") { */
                /*     if (isInteger(cell)) { */
                /*         columnTypes[currentColumn] = "int"; */
                /*     } else if (isDouble(cell)) { */
                /*         columnTypes[currentColumn] = "float"; */
                /*     } else { */
                /*         columnTypes[currentColumn] = "string"; */
                /*     } */
                /* } else if (columnTypes[currentColumn] == "integer" && !isInteger(cell)) { */
                /*     columnTypes[currentColumn] = isDouble(cell) ? "float" : "string"; */
                /* } else if (columnTypes[currentColumn] == "float" && !isDouble(cell)) { */
                /*     columnTypes[currentColumn] = "string"; */
                /* } */

                ++colIndex;
            }
            ++rowCount;
            rowLine = cleanString(rowLine); // remove non-printable/control characters
            outputFile << rowLine << "\n";
        }

        // Write the new formatted header at the beginning of the output file
        std::string newHeader = formatHeader(columnNames, rowCount, columnTypes);
        newHeader = cleanString(newHeader);



        // Reopen the output file in "append and replace" mode
        outputFile.close();

        /* std::ofstream tempOutputFile(outputFilename, std::ios::in | std::ios::out); */
        /* tempOutputFile.seekp(0); // Move to the start of the file */
        /* tempOutputFile << newHeader << "\n"; */
        /* tempOutputFile.close(); */

        // Prepend the header by creating a temporary file
        std::ifstream oldOutputFile(outputFilename);
        std::ofstream newOutputFile("temp_output.csv");
        newOutputFile << newHeader << "\n"; // Write the new header

        // Append the rest of the original output file
        std::string line;
        while (std::getline(oldOutputFile, line)) {
            newOutputFile << line << "\n";
        }

        // Close files
        oldOutputFile.close();
        newOutputFile.close();

        // Replace the original output file with the updated file
        std::remove(outputFilename.c_str());
        std::rename("temp_output.csv", outputFilename.c_str());
        
        // Output the column names and count
        std::cout << "Number of columns: " << columnNames.size() << "\n";
        std::cout << "Number of rows: " << rowCount << "\n";
        std::cout << "Column names:\n";
        for (const auto& name : columnNames) {
            std::cout << name << "\n";
        }
        std::cout << "Column types:\n";
        for (const auto& t: columnTypes) {
            std::cout << t << "\n";
        }

        file.close();

        std::cout << "Header replaced successfully in " << filename << "\n";

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }

    /* try { */
    /*     // Create a CSV reader */
    /*     const int n = columnNames.size(); */
    /*     const char * c_filename= filename.c_str();  // Convert to C-style string */

    /*     io::CSVReader<3> df("superstore.csv"); // Use 0 to handle unknown column count dynamically */
        
    /*     df.read_header(io::ignore_extra_column, "Row ID", "Order ID", "Order Date"); */
    /*     // Read the headers dynamically */
    /*     std::cout << "Headers:\n"; */
    /*     for (const auto& col : columnNames) { */
    /*         std::cout << col << " "; */
    /*     } */
    /*     std::cout << "\n"; */

    /*     // Dynamically read each row as a vector of strings */
    /*     /1* std::vector<std::string> row; *1/ */
    /*     /1* while (df.read_row(row)) { *1/ */
    /*     /1*     for (size_t i = 0; i < row.size(); ++i) { *1/ */
    /*     /1*         std::cout << columnNames[i] << ": " << row[i]; *1/ */
    /*     /1*         if (i < row.size() - 1) std::cout << ", "; *1/ */
    /*     /1*     } *1/ */
    /*     /1*     std::cout << '\n'; *1/ */
    /*     /1* } *1/ */
    /* } catch (const std::exception &ex) { */
    /*     std::cerr << "Error reading the CSV file: " << ex.what() << '\n'; */
    /*     return 1; */
    /* } */

    return 0;
}

