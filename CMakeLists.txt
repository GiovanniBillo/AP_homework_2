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

# Add the debugging flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

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

