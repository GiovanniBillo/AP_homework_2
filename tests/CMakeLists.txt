cmake_minimum_required(VERSION 3.14)

project(statistics LANGUAGES CXX)

# GoogleTest requires at least C++14.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler flags for debugging
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -Wpedantic")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic")

include(FetchContent)

# Fetch GoogleTest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Fetch the DataFrame library
FetchContent_Declare(
    DataFrame
    GIT_REPOSITORY https://github.com/hosseinmoein/DataFrame.git
)
FetchContent_MakeAvailable(DataFrame)

# Add the tests executable
add_executable(tests test_statsmodule.cpp)

if(DEFINED DATA_DIR)
  message(STATUS "Using user-specified data directory: ${DATA_DIR}")
else()
  set(DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)
  message(STATUS "Using default data directory: ${DATA_DIR}")
endif()

add_definitions(-DDATA_DIR=\"${DATA_DIR}\")

# Include directories
target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

# Print the paths being added to the include directories
message("Include path 1: ${CMAKE_CURRENT_SOURCE_DIR}/../include")
message("Include path 2: ${CMAKE_SOURCE_DIR}/../src")

# Link libraries
target_link_libraries(tests PRIVATE
    GTest::gtest_main
    DataFrame::DataFrame
)

enable_testing()
include(GoogleTest)
gtest_discover_tests(tests)





