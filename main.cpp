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

    const auto &discountColumn = dfw.operator$<double>("Discount"); 
    std::cout << discountColumn[1] << discountColumn[2] << discountColumn[3] << std::endl;
    auto sd = dfw.StandardDeviation<double>("Discount");
    auto mean = dfw.Mean<double>("Discount");
    std::cout << sd<< std::endl;
    std::cout << mean << std::endl;


    /* StrDataFrame    ibm_df; */

    /* // Also, you can load data into a DataFrame from a file, supporting a few different formats. If the file cannot be found, */
    /* // an exception will be thrown. If the DataFrame data directory is your current directory when running this, it should */
    /* // work fine. */
    /* // */
    /* ibm_df.read("superstore_updated.csv", io_format::csv2); */
    
    return (0);
}

// Local Variables:
// mode:C++
// tab-width:4
// c-basic-offset:4
// End:
