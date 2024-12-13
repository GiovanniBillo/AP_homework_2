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

namespace Toolbox{
    namespace dfw{  

        using namespace hmdf;

        template <typename T>
            class DataFrameWrapper {
                private:
                    std::filesystem::path data_directory;
                    std::string inputFilename;
                    std::string outputFilename;

                    hmdf::StdDataFrame<T> df;

                    // Dictionary to store column names and their types
                    std::vector<std::pair<std::string, std::string>> columnInfo;

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


                        // Process the data to put it in the right format for DataFrame
                        CSVParser parser;

                        std::filesystem::path if_path = data_directory / inputFilename;
                        std::filesystem::path of_path = data_directory / outputFilename;
                        parser.parse(if_path.string(), of_path.string()); 

                        columnInfo = parser.getColumnTypes();

                        // DataFrame requires file input for read to be in C-style string
                        const char* c_path = of_path.c_str();

                        df.read(c_path, io_format::csv2);

                    }

                    size_t getColIndex(const char * columnName){
                        int index = -1; // skip the initial index column 
                        for (const auto& [name, type] : columnInfo) {  
                            if (name == columnName) {
                                return index;  
                            }
                            ++index;  
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
                            if (col_index >= columnInfo.size()) {
                                throw std::out_of_range("Invalid range: column index is out of bounds"); 
                            } 
                            return df.template get_column<CT>(col_index);
                        }

                    // return a column entry
                    template <typename CT>
                        const auto columns(std::size_t col_index, std::size_t row_index) const {
                            if (col_index >= columnInfo.size()) {
                                throw std::out_of_range("Invalid range: column or row index is out of bounds");
                                exit(1);
                            }
                            const auto &column =  df.template get_column<CT>(col_index);
                            CT entry = column[row_index]; 
                            return entry;
                        }

                    // return a column slice 
                    template <typename CT>
                        const auto columns(std::size_t col_index, std::size_t start, std::size_t stop) const {
                            // Check that start and stop are within bounds
                            const auto &column =  df.template get_column<CT>(col_index);
                            if (start >= column.size() || stop >= column.size() || start > stop) { 
                                throw std::out_of_range("Invalid slice range: start or stop index is out of bounds"); 
                            }
                            std::vector<CT> slice;
                            for (size_t i = start; i <= stop; ++i){
                                slice.push_back(column[i]); 
                            }

                            return slice;
                        }

                    template <typename CT>
                        auto StandardDeviation(const char* columnName) {
                            StdVisitor<CT, T> visitor;

                            df.template visit<CT>(columnName, visitor);
                            return visitor.get_result();  // Return the result of the visitor
                        }

                    template <typename CT>
                        auto Mean(const char* columnName) {
                            MeanVisitor<CT, T> visitor;

                            df.template visit<CT>(columnName, visitor);
                            return visitor.get_result();  // Return the result of the visitor
                        }

                    template <typename CT>
                        auto Variance(const char* columnName) {
                            StdVisitor<CT, T> visitor;

                            df.template visit<CT>(columnName, visitor);
                            return pow(visitor.get_result(), 2);  // Return the result of the visitor
                        }

                    template <typename CT>
                        auto Median(const char* columnName) {
                            MedianVisitor<CT, T> visitor;

                            df.template visit<CT>(columnName, visitor);
                            return visitor.get_result();  // Return the result of the visitor
                        }

                    template <typename CT>
                        auto Correlation(const char* columnName1, const char * columnName2) {
                            CorrVisitor<CT, T> visitor;

                            df.template single_act_visit<CT, CT>(columnName1, columnName2, visitor);
                            return visitor.get_result();  // Return the result of the visitor
                        }

                    template <typename CT>
                        auto frequencyCount(const char * columnName) {
                            auto result =  df. template value_counts<CT>(columnName);
                            auto name_index = result.get_index();
                            const std::vector<unsigned long> & counts = result. template get_column<size_t>("counts"); 
                            std::unordered_map<CT, unsigned long> index_to_count;

                            for (unsigned int i = 0; i < name_index.size(); ++i) {
                                index_to_count[name_index[i]] = counts[i];
                            }
                            return index_to_count;  
                        }

                    // Friend function to overload <<
                    friend std::ostream& operator<<(std::ostream& os, const StdDataFrame<T>& df) {
                        df.template write<std::ostream, double, T>(std::cout);

                        return os;
                    }

                    template <typename CT>
                        auto Classify(const char* columnName1, 
                                const std::vector<std::string> categories, 
                                const std::vector<std::function<bool(CT)>>& conditions) {
                            ClassifyVisitor<T, CT> visitor(categories, conditions);

                            df.template visit<CT>(columnName1, visitor); 
                            return visitor.get_result();  // Return the result of the visitor
                        }
#ifdef INTERPOLATION_MODULE
                    template <typename CT>
                    auto Interpolate(const char* columnName1, const char * columnName2, 
                            const std::vector<std::string> categories, 
                            const std::vector<std::function<bool(CT)>>& conditions) {
                        InterpolationVisitor<T, CT> visitor(categories, conditions);

                        df.template visit<CT>(columnName1, visitor); 
                        return visitor.get_result();  // Return the result of the visitor
                    }
#endif
                    void getInfo(){
                        try {
                            auto desc = df.template describe<double, int>(); 

                            // Write to console
                            desc.template write<std::ostream, double>(std::cout, io_format::csv2);

                            // Write to a text file
                            std::ofstream outFile(inputFilename+"_info.txt");
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

    }
}

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
