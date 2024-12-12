cmake_minimum_required(VERSION 3.14)

project(statistics LANGUAGES CXX)

# GoogleTest requires at least C++14.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler flags for debugging
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall -Wpedantic")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

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

# Fetch the ALGLIB library
FetchContent_Declare(
   alglib 
    GIT_REPOSITORY https://github.com/hjkuijf/ALGLIB
    # GIT_TAG v1.18.0 # Uncomment to use a specific version or tag
)

# Ensure the ALGLIB library is downloaded and built
FetchContent_MakeAvailable(alglib)

# Add the tests executable
add_executable(tests test_statsmodule.cpp)

# If ALGLIB is header-only or doesn't provide a library target:
target_include_directories(tests PRIVATE ${alglib_SOURCE_DIR}/src)

# If ALGLIB provides a CMake target (for example, alglib::alglib), link it
# Uncomment the line below if a proper CMake target is available
# target_link_libraries(main PRIVATE alglib::alglib)

# Manually add the source files if ALGLIB doesn't provide a CMake target
file(GLOB ALGLIB_SRC 
    ${alglib_SOURCE_DIR}/src/*.cpp
)


# Link the source files to your project if ALGLIB doesn't have a CMake target
target_sources(tests PRIVATE ${ALGLIB_SRC})


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
    ${alglib_SOURCE_DIR}/src
)
message("ALGLIB SOURCE DIR IS: " ${alglib_SOURCE_DIR}/src)

# Print the paths being added to the include directories
message("Include path 1: ${CMAKE_CURRENT_SOURCE_DIR}/../include")
message("Include path 2: ${CMAKE_SOURCE_DIR}/../src")

# Link libraries
target_link_libraries(tests PRIVATE
    GTest::gtest_main
    DataFrame::DataFrame
)
option(INTERPOLATION_MODULE "Include the InterpolationWrapper and Interpolate custom visitor" ON)
# Conditionally include a library and headers
if(INTERPOLATION_MODULE)
    message(STATUS "Include the InterpolationWrapper and Interpolate custom visitor")
    add_compile_definitions(INTERPOLATION_MODULE)
    target_compile_definitions(tests PRIVATE INTERPOLATION_MODULE)
else()
    message(STATUS "Interpolation module is not included")
endif()




# Message to confirm the source directory of ALGLIB
message(STATUS "ALGLIB source dir: ${alglib_SOURCE_DIR}")

enable_testing()
include(GoogleTest)
gtest_discover_tests(tests)






