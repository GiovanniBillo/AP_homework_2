// libraries imported from DataFrame 
#include <DataFrame/DataFrame.h>                   // Main DataFrame header
#include <DataFrame/DataFrameStatsVisitors.h>      // Statistical algorithms

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include "DataFrameWrapper.hpp"

using namespace Toolbox;

int main(int, char *[])  {

    Toolbox::dfw::DataFrameWrapper<int> dfw("r.csv", "o.csv"); // modify the type template according to your index column (if not an already formatted dataset, it will be assigned an int type to the first column, usually the Row index)
    dfw.loadAndReadFile();
    const char * columnName1 = "Discount"; // index = 4
    const char * columnName2 = "Profit"; //index = 6
    std::cout << "Retrieving each information by itself" << std::endl;
    const auto &discountColumn = dfw.columns<double>(columnName1); 
    size_t discountIndex = dfw.getColIndex(columnName1); 
    const auto &discountColumn_byindex = dfw.columns<double>(discountIndex); 
    const auto &discountColumn_entry_byindex = dfw.columns<double>(4, 1); // retrieve the first entry of the discount column  
    const auto &discountColumn_slice= dfw.columns<double>(4, 1, 4); // retrieve the first entry of the discount column  
    std::cout << "name and then index:"<< discountColumn[1] << discountColumn[2] << discountColumn[3] << std::endl;
    std::cout << "by column index" << discountColumn_byindex[1] << discountColumn_byindex[2] << discountColumn_byindex[3] << std::endl;
    std::cout << "indexing into a slice" << discountColumn_slice[0] << discountColumn_slice[1] << discountColumn_slice[2] << std::endl;

    auto sd = dfw.StandardDeviation<double>(columnName1);
    auto mean = dfw.Mean<double>(columnName1);
    auto variance = dfw.Variance<double>(columnName1);
    auto median= dfw.Median<double>(columnName1);
    auto correlation = dfw.Correlation<double>(columnName1, columnName2);
    auto frequencyCounts = dfw.frequencyCount<double>(columnName1);

    std::cout << "Standard Deviation: " << sd << std::endl;
    std::cout << "Mean: " << mean << std::endl;
    std::cout << "Variance: " << variance << std::endl;
    std::cout << "Median: " << median << std::endl;
    std::cout << "Correlation between " << columnName1 << "and" << columnName2 <<" : " << correlation << std::endl;
    std::cout << "Frequency counts for " << columnName1 << " : " << std::endl;
    std::cout << frequencyCounts << std::endl; 
    std::cout << "summary function:"<< std::endl;
    dfw.getInfo();

    std::cout << "There is also the possibility to classify column data based on specific criteria (passed as lambda functions).\n Usage example: classify profit data as below or above average" << std::endl;

    auto mean_profit = dfw.Mean<double>(columnName2);    
    const auto &profits = dfw.columns<double>(columnName2);

    std::vector<std::string> categories = {"Below Mean", "Above Mean"};

    const std::vector<std::function<bool(double)>> conditions = {
        [mean_profit](double value) { return value < mean_profit; },  // Below Mean
        [mean_profit](double value) { return value >= mean_profit; }  // Above Mean
    };

    std::vector<std::string> classifications = dfw.Classify<double>(columnName2, categories, conditions);
    std::cout << "Average profit:" << mean_profit << std::endl; 
    for (size_t i = 0; i < 5; ++i) {
        /* std::cout << classifications[i] << std::endl; */
        std::cout << "Profit: " << profits[i] << " -> " << classifications[i] << std::endl;
    }

    return (0);
}


