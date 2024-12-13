#include <gtest/gtest.h>
/* #include <DataFrame/DataFrame.h>                   // Main DataFrame header */
/* #include <DataFrame/DataFrameStatsVisitors.h>      // Statistical algorithms */
#include "DataFrameCustomVisitors.hpp"
#include "DataFrameWrapper.hpp"
#include "InterpolateWrapper.hpp"
#include <filesystem>
#include <string>
#include <stdexcept>

using namespace Toolbox;


TEST(CSVParserTest, DifferentDatasets) {
    Toolbox::dfw::DataFrameWrapper<std::string> bankData("bank.csv", "bank_out.csv");
    bankData.loadAndReadFile();
    bankData.getInfo();

    Toolbox::dfw::DataFrameWrapper<std::string> IBMData("IBM.csv", "IBM_out.csv");
    IBMData.loadAndReadFile();
    IBMData.getInfo();

    Toolbox::dfw::DataFrameWrapper<std::string> EcomData("ecom.csv", "ecom_out.csv");
    EcomData.loadAndReadFile();
    EcomData.getInfo();

        // Check if the expected files were created
    std::ifstream bankFile("bank.csv_info.txt");
    std::ifstream IBMFile("IBM.csv_info.txt");
    std::ifstream ecomFile("ecom.csv_info.txt");

    // Check if the files exist
    EXPECT_TRUE(bankFile.is_open()) << "bank.csv_info.txt was not created.";
    EXPECT_TRUE(IBMFile.is_open()) << "IBM.csv_info.txt was not created.";
    EXPECT_TRUE(ecomFile.is_open()) << "ecom.csv_info.txt was not created.";

    // Close files (optional, good practice)
    bankFile.close();
    IBMFile.close();
    ecomFile.close();
}

TEST(IteratorsTest, GetColIndex) {
    Toolbox::dfw::DataFrameWrapper<int> dfw("r.csv", "o.csv");
    dfw.loadAndReadFile();
    // Placeholder test that will fail
    const char * columnName1 = "Discount"; // index = 4
    const char * columnName2 = "Profit"; //index = 6
    size_t discountIndex = dfw.getColIndex(columnName1);
    size_t profitIndex = dfw.getColIndex(columnName2);

    EXPECT_EQ(discountIndex, 4);
    EXPECT_EQ(profitIndex, 6);

    // Extra debugging: Triggered only if the test fails
    if (HasFailure()) {
        std::cerr << "Test failed. Debugging information:\n";
    }
}


TEST(IteratorsTest, ColumnsByIndex) {
    Toolbox::dfw::DataFrameWrapper<int> dfw("r.csv", "o.csv");
    dfw.loadAndReadFile();


    EXPECT_THROW(
      {
        dfw.columns<double>(10);  // Invalid column index 10
      },
      std::out_of_range
    );

    if (HasFailure()) {
        std::cerr << "Test failed. Debugging information:\n";
    }
}

TEST(IteratorsTest, ColumnsByIndexAndRowIndex) {
    Toolbox::dfw::DataFrameWrapper<int> dfw("r.csv", "o.csv");
    dfw.loadAndReadFile();
    
    EXPECT_THROW(
      {
        dfw.columns<double>(10, 1);  // Invalid column index 10
      },
      std::out_of_range
    );

    if (HasFailure()) {
        std::cerr << "Test failed. Debugging information:\n";
    }
}

TEST(IteratorsTest, ColumnsByIndexAndSlice) {
    Toolbox::dfw::DataFrameWrapper<int> dfw("r.csv", "o.csv");
    dfw.loadAndReadFile();

    try {
        const auto& discountColumn_slice = dfw.columns<double>(4, 1, 4);  // Valid slice from row 1 to row 4 (inclusive)
        
        // Verify the slice is correct
        EXPECT_EQ(discountColumn_slice.size(), 4);  // Ensure the size is correct
        EXPECT_EQ(discountColumn_slice[0], 0.80);     // Example check for the first element

    } catch (...) {
        FAIL() << "Valid slice access failed unexpectedly";  // Fail the test if any exception occurs unexpectedly
    }

    EXPECT_THROW(
      {
        dfw.columns<double>(4, 1, 102);  // Invalid column index 10
      },
      std::out_of_range
    );
}

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

