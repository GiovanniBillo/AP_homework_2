#include <gtest/gtest.h>
/* #include <DataFrame/DataFrame.h>                   // Main DataFrame header */
/* #include <DataFrame/DataFrameStatsVisitors.h>      // Statistical algorithms */
#include "DataFrameCustomVisitors.hpp"
#include "Wrapper.hpp"
#include <filesystem>


TEST(CustomVisitorsTests, ClassifyVisitor) {
    /* auto mean_profit = dfw.Mean<double>(columnName2); */    
    /* const auto &profits = dfw.operator$<double>(columnName2); */

    const std::vector<std::string> categories = {"Below Mean", "Above Mean"};
    
    double mean_profit = 29.7693;
    const std::vector<std::function<bool(double)>> conditions = {
        [mean_profit](double value) { return value < mean_profit; },  // Below Mean
        [mean_profit](double value) { return value >= mean_profit; }  // Above Mean
    };
    
    std::vector<std::string> classifications = {
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Above Mean",
    "Below Mean",
    "Below Mean",
    "Above Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Above Mean",
    "Above Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Above Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Above Mean",
    "Above Mean",
    "Above Mean",
    "Below Mean",
    "Above Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Above Mean",
    "Below Mean",
    "Above Mean",
    "Below Mean",
    "Above Mean",
    "Below Mean",
    "Below Mean",
    "Above Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Above Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Above Mean",
    "Below Mean",
    "Below Mean",
    "Above Mean",
    "Below Mean",
    "Above Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Above Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Above Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Below Mean",
    "Above Mean",
    "Above Mean",
    "Below Mean",
    "Above Mean",
    "Below Mean"
};

    /* std::vector<std::string> classifications = dfw.classify<double>(columnName2, categories, conditions); */
    
    // Create the visitor inline
    // Sales Dataset has an int index type and the "Profit" column has a "double" type
    ClassifyVisitor<int, double> visitor(categories, conditions);
    
    hmdf::StdDataFrame<int> df;
    std::filesystem::path data_directory = DATA_DIR;
    std::filesystem::path path = data_directory / "o.csv";
    df.read(path.c_str(), hmdf::io_format::csv2);
     
    // Perform operations
    df.template visit<double>("Profit", visitor); // what;s the difference between this and single_act_visit?
    auto result = visitor.get_result(); 


    EXPECT_EQ(classifications,
              result);

        // Extra debugging: Triggered only if the test fails
    if (HasFailure()) {
        std::cerr << "Test failed. Debugging information:\n";

        // Check sizes
        if (classifications.size() != result.size()) {
            std::cerr << "Size mismatch: Expected " << classifications.size()
                      << ", Got " << result.size() << "\n";
        } else {
            // Print mismatched elements
            for (size_t i = 0; i < classifications.size(); ++i) {
                if (classifications[i] != result[i]) {
                    std::cerr << "Mismatch at index " << i 
                              << ": Expected [" << classifications[i] 
                              << "], Got [" << result[i] << "]\n";
                }
            }
        }
}
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

