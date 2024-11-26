cmake_minimum_required(VERSION 3.22)
project(HelloWorld LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(DataFrame REQUIRED)
add_executable(hello_world hello_world.cc)

set(DATAFRAME_HEADER_DIR /usr/local/include/DataFrame)
set(DATAFRAME_LIBRARY  /usr/local/lib/libDataFrame.a)  # Adjust for libmuparserx.so if using shared library


# Add the include directory for the DataFrame headers
target_include_directories(hello_world PRIVATE ${DATAFRAME_HEADER_DIR})

target_link_libraries(hello_world PRIVATE ${DATAFRAME_LIBRARY})
