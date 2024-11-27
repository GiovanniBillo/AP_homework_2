# cmake_minimum_required(VERSION 3.22)
# project(main LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # find_package(DataFrame REQUIRED)
# add_executable(main main.cpp)

# set(DATAFRAME_HEADER_DIR /usr/local/include/DataFrame)
# set(DATAFRAME_LIBRARY  /usr/local/lib/libDataFrame.a)  


# # Add the include directory for the DataFrame headers
# target_include_directories(main PRIVATE ${DATAFRAME_HEADER_DIR})

# target_link_libraries(main PRIVATE ${DATAFRAME_LIBRARY})


cmake_minimum_required(VERSION 3.22)
project(main LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
add_executable(main main.cpp)

# Link the DataFrame library
target_link_libraries(main PRIVATE DataFrame::DataFrame)

