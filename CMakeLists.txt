# cmake_minimum_required(VERSION 3.22)
# project(main LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# if(DEFINED DATA_DIR)
#   message(STATUS "Using user-specified data directory: ${DATA_DIR}")
# else()
#   set(DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)
#   message(STATUS "Using default data directory: ${DATA_DIR}")
# endif()

# add_definitions(-DDATA_DIR=\"${DATA_DIR}\")

# include(FetchContent)

# # Fetch the DataFrame library
# FetchContent_Declare(
#     DataFrame
#     GIT_REPOSITORY https://github.com/hosseinmoein/DataFrame.git
#     # GIT_TAG v1.18.0 # Replace with the desired stable version tag
# )

# # Ensure the DataFrame library is downloaded and built
# FetchContent_MakeAvailable(DataFrame)

# # Add the executable
# add_executable(main main.cpp)

# # Link the DataFrame library
# target_link_libraries(main PRIVATE DataFrame::DataFrame)
cmake_minimum_required(VERSION 3.22)
project(main LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(DEFINED DATA_DIR)
  message(STATUS "Using user-specified data directory: ${DATA_DIR}")
else()
  set(DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)
  message(STATUS "Using default data directory: ${DATA_DIR}")
endif()

add_definitions(-DDATA_DIR=\"${DATA_DIR}\")

include_directories(${CMAKE_SOURCE_DIR}/include)


# Add the debugging flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g") # !!!TODO: modify with optimizations when releasing

include(FetchContent)

# Fetch the DataFrame library
FetchContent_Declare(
    DataFrame
    GIT_REPOSITORY https://github.com/hosseinmoein/DataFrame.git
    # GIT_TAG v1.18.0 # Replace with the desired stable version tag
)

# Ensure the DataFrame library is downloaded and built
FetchContent_MakeAvailable(DataFrame)

# Add the executable
add_executable(main src/main.cpp)

# Link the DataFrame library
target_link_libraries(main PRIVATE DataFrame::DataFrame)

# # Separate testing configuration
# option(BUILD_TESTING "Enable testing" ON)

# # Add the tests subdirectory
# add_subdirectory(tests)

# if(BUILD_TESTING)
#     # Enable testing
#     enable_testing()

#     # Fetch Google Test
#     FetchContent_Declare(
#         googletest
#         GIT_REPOSITORY https://github.com/google/googletest.git
#         GIT_TAG release-1.12.1 # Use the latest stable tag
#     )

#     FetchContent_MakeAvailable(googletest)

#     # Add the test executable
#     add_executable(tests tests/test_main.cpp)

#     # Link Google Test and your project's dependencies to the test executable
#     target_link_libraries(tests PRIVATE 
#         GTest::gtest 
#         GTest::gtest_main 
#         DataFrame::DataFrame
#     )

#     # Register tests
#     include(GoogleTest)
#     gtest_discover_tests(tests)
# endif()

