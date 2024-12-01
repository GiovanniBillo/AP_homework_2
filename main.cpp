// libraries imported from DataFrame 
#include <DataFrame/DataFrame.h>                   // Main DataFrame header
#include <DataFrame/DataFrameStatsVisitors.h>      // Statistical algorithms

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include "Wrapper.hpp"


int main(int, char *[])  {

    DataFrameWrapper<int> dfw;
    dfw.loadAndReadFile();
    const char * columnName1 = "Discount";
    const char * columnName2 = "Profit";

    const auto &discountColumn = dfw.operator$<double>(columnName1); 
    std::cout << discountColumn[1] << discountColumn[2] << discountColumn[3] << std::endl;
    auto sd = dfw.StandardDeviation<double>(columnName1);
    auto mean = dfw.Mean<double>(columnName1);
    auto variance = dfw.Variance<double>(columnName1);
    auto median= dfw.Median<double>(columnName1);
    auto correlation = dfw.Correlation<double>(columnName1, columnName2);
    auto frequencyCounts = dfw.frequencyCount<double>(columnName1);
    /* const auto &count_column = frequencyCounts.get_column<size_t>("counts"); */ 

    std::cout << "Standard Deviation: " << sd << std::endl;
    std::cout << "Mean: " << mean << std::endl;
    std::cout << "Variance: " << variance << std::endl;
    std::cout << "Median: " << median << std::endl;
    std::cout << "Correlation between " << columnName1 << "and" << columnName2 <<" : " << correlation << std::endl;
    std::cout << "Frequency counts for " << columnName1 << " : " << std::endl;
    /* std::cout << frequencyCounts << std::endl; */ // TODO solve bug that srops anything else from printing

    // function to classify column data
    // Usage example: classify profit data as below or above average
    auto mean_profit = dfw.Mean<double>(columnName2);    
    const auto &profits = dfw.operator$<double>(columnName2);

    std::vector<std::string> categories = {"Below Mean", "Above Mean"};

    const std::vector<std::function<bool(double)>> conditions = {
        [mean](double value) { return value < mean; },  // Below Mean
        [mean](double value) { return value >= mean; }  // Above Mean
    };

    std::vector<std::string> classifications = dfw.classify<double>(columnName2, categories, conditions);


    for (size_t i = 0; i < profits.size(); ++i) {
        std::cout << "Profit: " << profits[i] << " -> " << classifications[i] << std::endl;
    }

    return (0);
}


