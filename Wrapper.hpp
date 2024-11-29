// libraries imported from DataFrame 
#include <DataFrame/DataFrame.h>                   // Main DataFrame header
#include <DataFrame/DataFrameStatsVisitors.h>      // Statistical algorithms

#include <iostream>
#include <fstream>
/* #include <ifstream> */
#include <string>
#include <filesystem>

#include "parser.hpp"

// -----------------------------------------------------------------------------

// DataFrame library is entirely under hmdf name-space
//
using namespace hmdf;

// A DataFrame with ulong index type
//
using ULDataFrame = StdDataFrame<unsigned long>;

// A DataFrame with string index type
//
using StrDataFrame = StdDataFrame<std::string>;

// A DataFrame with DateTime index type
//
using DTDataFrame = StdDataFrame<DateTime>;


template <typename T>
class DataFrameWrapper {
private:
    std::filesystem::path data_directory;
    std::string inputFilename;

    // Generalized DataFrame using template
    // TODO: solve this issue to make template adapt to any index type
    hmdf::StdDataFrame<T> df;

    // Dictionary to store column names and their types
    std::unordered_map<std::string, std::string>* columnInfo = nullptr; // Pointer to columnTypes
 
public:
    explicit DataFrameWrapper() 
        : data_directory(DATA_DIR) {  // Initialize data_directory with DATA_DIR macro
        std::cout << "Wrapper initialized!" << std::endl;
        std::cout << "Please provide the name of the input file in the chosen data directory: ";
        std::cin >> inputFilename;
    }

    void loadAndReadFile() {

        /* // Create an instance of StrDataFrame */
        /* hmdf::StdDataFrame<std::string> df; */
        
        // Process the data to put it in the right format for DataFrame
        CSVParser parser;
        std::string outputFilename = inputFilename + "_out";

        std::filesystem::path if_path = data_directory / inputFilename;
        parser.parse(if_path.string(), outputFilename); 
        
        columnInfo = parser.getColumnTypes();
        if (!columnInfo) {
            throw std::runtime_error("Column types not set. Please initialize columnInfo.");
        }

        // get the file path 
        /* std::filesystem::path of_path = outputFilename; */

        // DataFrame requires file input for read to be in C-style string
        const char* c_path = outputFilename.c_str();
        std::string indexType = columnInfo->at("INDEX");

                                                 
        try {
            df.read(c_path, hmdf::io_format::csv2);  // Adjust to your DataFrame library's API
            std::cout << "File read successfully!" << std::endl;
            std::cout << "Loaded DataFrame has " 
                      << df.get_index().size() 
                      << " rows." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: Could not read the file. " << e.what() << std::endl;
        }
    
        std::cout << "Column types:\n";
        for (const auto& t: *columnInfo) {
            std::cout << t << "\n";
        }
        std::cout << indexType << std::endl;

    }

/* void instantiateDataFrame(std::unique_ptr<hmdf::StdDataFrameBase> df, std::string indexType){ */
/*     df = std::make_unique<hmdf::StdDataFrame<indexType>>() */
/*     } */

    void getInfo(){}
};
