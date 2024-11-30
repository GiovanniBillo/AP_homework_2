// libraries imported from DataFrame 
#include <DataFrame/DataFrame.h>                   // Main DataFrame header
#include <DataFrame/DataFrameStatsVisitors.h>      // Statistical algorithms

#include <iostream>
#include <fstream>
/* #include <ifstream> */
#include <string>
#include <filesystem>

#include "parser.hpp"
/* #include "initializer.hpp" */

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

// Alias for a type-erased DataFrame
using DataFrameVariant = std::variant<
    hmdf::StdDataFrame<int>,
    hmdf::StdDataFrame<double>,
    hmdf::StdDataFrame<std::string>>;

template <typename T>
class DataFrameWrapper {
private:
    std::filesystem::path data_directory;
    std::string inputFilename;
    std::string outputFilename;

    hmdf::StdDataFrame<T> df;

    // Dictionary to store column names and their types
    std::unordered_map<std::string, std::string>* columnInfo = nullptr; // Pointer to columnTypes
 
public:
    explicit DataFrameWrapper() 
        : data_directory(DATA_DIR) {  // Initialize data_directory with DATA_DIR macro
        std::cout << "Wrapper initialized!" << std::endl;
        std::cout << "Please provide the name of the input file in the chosen data directory: ";
        std::cin >> inputFilename;
        std::cout << "Please provide the name of the desired output filename: ";
        std::cin >> outputFilename;
    }

    void loadAndReadFile() {

        /* // Create an instance of StrDataFrame */
        /* hmdf::StdDataFrame<std::string> df; */
        
        // Process the data to put it in the right format for DataFrame
        CSVParser parser;
        /* std::string outputFilename = inputFilename + "_out"; */

        std::filesystem::path if_path = data_directory / inputFilename;
        std::filesystem::path of_path = data_directory / outputFilename;
        parser.parse(if_path.string(), of_path.string()); 
        
        columnInfo = parser.getColumnTypes();
        if (!columnInfo) {
            throw std::runtime_error("Column types not set. Please initialize columnInfo.");
        }

        std::cout << "Column types:\n";
        for (const auto& t: *columnInfo) {
            std::cout << t << "\n";
        }

        // DataFrame requires file input for read to be in C-style string
        const char* c_path = of_path.c_str();
         

        df.read(c_path, io_format::csv2);
        
    }
    
    template <typename CT>
    const auto operator$(const char * columnName) const {
        return df.template get_column<CT>(columnName);
    }
    
    /* template <typename CT> */
    /* const auto mean(const char * columnName, visitor = MeanVisitor<double, std::string>){ */
    /*    df.visit<CT>(columnName, visitor); */
    /*    visitor.get_result(); */

    /* } */
    template <typename CT>
    auto StandardDeviation(const char* columnName) {
        // Create the visitor inline
        StdVisitor<CT, T> visitor;

        // Perform operations
        df.template visit<CT>(columnName, visitor);
        return visitor.get_result();  // Return the result of the visitor
    }

    template <typename CT>
    auto Mean(const char* columnName) {
        // Create the visitor inline
        MeanVisitor<CT, T> visitor;

        // Perform operations
        df.template visit<CT>(columnName, visitor);
        return visitor.get_result();  // Return the result of the visitor
    }


    


    void getInfo(){}
};



