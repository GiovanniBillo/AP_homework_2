#include <gtest/gtest.h>
#include "wrapper.hpp"


TEST(InterpolationTests, Linear) {
    //TODO 
    EXPECT_EQ(1, // just a placeholder that always fails
              2);

    // Extra debugging: Triggered only if the test fails
    if (HasFailure()) {
        std::cerr << "Test failed. Debugging information:\n";
    }
}

// TEST GROUP for Lagrange Interpolator
TEST(LagrangeInterpolatorTests, Casual) {
    // TODO 
    EXPECT_EQ(1, 2);  // This will always fail

    // Extra debugging: Triggered only if the test fails
    if (HasFailure()) {
        std::cerr << "Test failed. Debugging information:\n";
    }
}

TEST(LagrangeInterpolatorTests, Equidistant) {
    // TODO 
    EXPECT_EQ(1, 2);  // This will always fail

    // Extra debugging: Triggered only if the test fails
    if (HasFailure()) {
        std::cerr << "Test failed. Debugging information:\n";
    }
}

TEST(LagrangeInterpolatorTests, Chebyshev) {
    // TODO 
    EXPECT_EQ(1, 2);  // This will always fail

    // Extra debugging: Triggered only if the test fails
    if (HasFailure()) {
        std::cerr << "Test failed. Debugging information:\n";
    }
}

// TEST GROUP for Spline Interpolator
TEST(SplineInterpolatorTests, Interpolation) {
    // TODO 
    EXPECT_EQ(1, 2);  // This will always fail

    // Extra debugging: Triggered only if the test fails
    if (HasFailure()) {
        std::cerr << "Test failed. Debugging information:\n";
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

