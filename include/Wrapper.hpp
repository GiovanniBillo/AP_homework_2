// libraries imported from DataFrame 
#include <DataFrame/DataFrame.h>                   // Main DataFrame header
#include <DataFrame/DataFrameStatsVisitors.h>      // Statistical algorithms

#include <iostream>
#include <fstream>
/* #include <ifstream> */
#include <string>
#include <filesystem>
#include <cmath>
#include <vector>
#include <functional>

#include "parser.hpp"
#include "DataFrameCustomVisitors.hpp"
/* #include "DataFrameCustomVisitors.hpp" */
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
    explicit DataFrameWrapper(const char * inputFilename, const char * outputFilename) 
        : data_directory(DATA_DIR), inputFilename(inputFilename), outputFilename(outputFilename) {
        std::cout << "Wrapper initialized!" << std::endl;
        std::cout << "provided input filename:" << inputFilename << std::endl;
        std::cout << "provided outputfilename:" << outputFilename << std::endl;
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

        /* std::cout << "Column types:\n"; */
        /* for (const auto& t: *columnInfo) { */
        /*     std::cout << t << "\n"; */
        /* } */

        // DataFrame requires file input for read to be in C-style string
        const char* c_path = of_path.c_str();
         

        df.read(c_path, io_format::csv2);
        
    }
    
    size_t getColIndex(const char * columnName){
        int index = 0; // Initialize index counter
        for (const auto& [key, value] : *columnInfo) {
            if (key == columnName) {
                return index; // Return the index when the key matches
            }
            ++index; // Increment the index for each element
        }
        throw std::runtime_error(std::string("Column \"") + columnName + "\" not found in ColumnInfo");

    }
        
    // return a column by name
    template <typename CT>
    const auto columns(const char * columnName) const {
        return df.template get_column<CT>(columnName);
    }
    
    // return a column by index
    template <typename CT>
    const auto columns(std::size_t col_index) const {
    /* if (col_index >= columnInfo->size()) { */
    /*      throw std::out_of_range("Invalid range: column index is out of bounds"); */
    /* } */
        return df.template get_column<CT>(col_index);
    }
    
    // return a column entry
    template <typename CT>
    const auto columns(std::size_t col_index, std::size_t row_index) const {
        const auto &column =  df.template get_column<CT>(col_index);
        /* if (col_index >= columnInfo->size() || row_index > column.size()) { */ //TODO: debug error handling  blocks
        /*     throw std::out_of_range("Invalid range: column or row index is out of bounds"); */
        /* } */
        CT entry = column[row_index]; 
        return entry;
    }

    // return a column slice 
    template <typename CT>
    const auto columns(std::size_t col_index, std::size_t start, std::size_t stop) const {
        const auto &column =  df.template get_column<CT>(col_index);
        // Check that start and stop are within bounds
        /* if (start >= column.size() || stop >= column.size() || start > stop) { */
        /*     throw std::out_of_range("Invalid slice range: start or stop index is out of bounds"); */
        /* } */
        std::vector<CT> slice;
        for (size_t i = start; i <= stop; ++i){
            slice.push_back(column[i]); 
        }
        
        return slice;
    }

    /* // return a series of values */ 
    /* template <typename CT> */
    /* const auto columns(std::size_t index1, std::size_t index2) const { */
    /*     const auto column =  df.template get_column<CT>(index); */
    /*     CT entry = column[index2]; */ 
    /*     return entry; */
    /* } */


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

    template <typename CT>
    auto Variance(const char* columnName) {
        // Create the visitor inline
        StdVisitor<CT, T> visitor;

        // Perform operations
        df.template visit<CT>(columnName, visitor);
        return pow(visitor.get_result(), 2);  // Return the result of the visitor
    }

    template <typename CT>
    auto Median(const char* columnName) {
        // Create the visitor inline
        MedianVisitor<CT, T> visitor;

        // Perform operations
        df.template visit<CT>(columnName, visitor);
        return visitor.get_result();  // Return the result of the visitor
    }

    template <typename CT>
    auto Correlation(const char* columnName1, const char * columnName2) {
        // Create the visitor inline
        CorrVisitor<CT, T> visitor;

        // Perform operations
        df.template single_act_visit<CT, CT>(columnName1, columnName2, visitor);
        return visitor.get_result();  // Return the result of the visitor
    }

    template <typename CT>
    auto frequencyCount(const char * columnName) {
        // Create the visitor inline
        auto result =  df. template value_counts<CT>(columnName);
        /* auto name_index = df.indices_; */ 
        auto name_index = result.get_index();
        const std::vector<unsigned long> & counts = result. template get_column<size_t>("counts"); 
        // Create a dictionary (unordered_map) to map name_index to counts
        std::unordered_map<CT, unsigned long> index_to_count;

        // Populate the dictionary
        for (unsigned int i = 0; i < name_index.size(); ++i) {
            index_to_count[name_index[i]] = counts[i];
        }
            return index_to_count;  // Return the result of the visitor
        }

    // Friend function to overload <<
    friend std::ostream& operator<<(std::ostream& os, const StdDataFrame<T>& df) {
            /* // Print column headers */
            /* for (const auto& col : df.columns) { */
            /*     os << std::setw(15) << col; // Adjust column width */
            /* } */
            /* os << std::endl; */

            /* // Print rows */
            /* for (const auto& row : df.data) { */
            /*     for (const auto& value : row) { */
            /*         os << std::setw(15) << value; // Align values */
            /*     } */
            /*     os << std::endl; */
            /* } */
            /* auto result = df.value_counts<double>("col_3"); */
            df.template write<std::ostream, double, T>(std::cout);

            return os;
        }

    template <typename CT>
    auto Classify(const char* columnName1, 
            const std::vector<std::string> categories, 
            const std::vector<std::function<bool(CT)>>& conditions) {
        // Create the visitor inline
        ClassifyVisitor<T, CT> visitor(categories, conditions);

        // Perform operations
        df.template visit<CT>(columnName1, visitor); 
        return visitor.get_result();  // Return the result of the visitor
    }

    void getInfo(){
            try {
        auto desc = df.template describe<double, int>(); // Filter for numerical types

        // Write to console
        desc.template write<std::ostream, double>(std::cout, io_format::csv2);

        // Write to a text file
        std::ofstream outFile("dataset_info.txt");
        if (outFile.is_open()) {
            std::ostream& outStream = outFile; // Bind to std::ostream
            desc.template write<std::ostream, double>(outStream, io_format::csv2);
            outFile.close();
        } else {
            std::cerr << "Error: Could not open the file for writing." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    }

    
};

// Overload operator<< for std::unordered_map
template <typename Key, typename Value>
std::ostream& operator<<(std::ostream& os, const std::unordered_map<Key, Value>& map) {
    os << "{ ";
    bool first = true; // Track whether this is the first element
    for (const auto& [key, value] : map) {
        if (!first) {
            os << ", "; // Add a comma only after the first element
        }
        os << key << ": " << value;
        first = false; // Set to false after the first element
    }
    os << " }";
    return os;
}

// Overload operator<< for std::vector<std::string>
std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& vec) {
    os << "[ ";
    bool first = true; // Track whether this is the first element
    for (const auto& item : vec) {
        if (!first) {
            os << ", "; // Add a comma only after the first element
        }
        os << "\"" << item << "\", ";
        first = false;
    }
    os << " ]";
    return os;
}
