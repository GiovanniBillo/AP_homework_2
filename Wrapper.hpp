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


class DataFrameWrapper {
private:
    std::filesystem::path data_directory;
    std::string filename;

    // Add the DataFrame as a member variable
    hmdf::StdDataFrame<int> df;

public:
    explicit DataFrameWrapper() 
        : data_directory(DATA_DIR) {  // Initialize data_directory with DATA_DIR macro
        std::cout << "Wrapper initialized!" << std::endl;
        std::cout << "Please provide the name of the input file in the chosen data directory: ";
        std::cin >> filename;
        this->loadAndReadFile();
    }

    void loadAndReadFile() {
        std::filesystem::path file_path = data_directory / filename;

        /* // Create an instance of StrDataFrame */
        /* hmdf::StdDataFrame<std::string> df; */

        const char* c_path = file_path.c_str();  // Convert to C-style string
        try {
            df.read(c_path, hmdf::io_format::csv2);  // Adjust to your DataFrame library's API
            std::cout << "File read successfully!" << std::endl;
            std::cout << "Loaded DataFrame has " 
                      << df.get_index().size() 
                      << " rows." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: Could not read the file. " << e.what() << std::endl;
        }
    }

    void getInfo(){
        

    }
};
