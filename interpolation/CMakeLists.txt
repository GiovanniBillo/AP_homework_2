# cmake_minimum_required(VERSION 3.22)
# project(main LANGUAGES CXX)

# set(CMAKE_CXX_STANDARD 23)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
#     message(STATUS "building DEBUG version")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -Wall -Wpedantic -Wextra")
#     set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_PROJECT_DIR}) 
# else() 
#     message(STATUS "building RELEASE version (default)")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g0 -O2 -DNDEBUG -Wall -Werror")
#     set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_PROJECT_DIR})
# endif()

# include(FetchContent)

# # Fetch the ALGLIB library
# FetchContent_Declare(
#    alglib 
#     GIT_REPOSITORY https://github.com/hjkuijf/ALGLIB
#     # GIT_TAG v1.18.0 # Uncomment to use a specific version or tag
# )

# # Ensure the ALGLIB library is downloaded and built
# FetchContent_MakeAvailable(alglib)


# target_link_libraries(main PRIVATE alglib::alglib)

# # Add the executable
# add_executable(main main.cpp)


# # If ALGLIB is header-only or doesn't provide a library target:
# target_include_directories(main PRIVATE ${alglib_SOURCE_DIR}/src)
# message(STATUS "Source dir:" ${alglib_SOURCE_DIR})

cmake_minimum_required(VERSION 3.22)
project(main LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message(STATUS "building DEBUG version")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -Wall -Wpedantic -Wextra")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_PROJECT_DIR}) 
else() 
    message(STATUS "building RELEASE version (default)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g0 -O2 -DNDEBUG -Wall -Werror")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_PROJECT_DIR})
endif()

include(FetchContent)

# Fetch the ALGLIB library
FetchContent_Declare(
   alglib 
    GIT_REPOSITORY https://github.com/hjkuijf/ALGLIB
    # GIT_TAG v1.18.0 # Uncomment to use a specific version or tag
)

# Ensure the ALGLIB library is downloaded and built
FetchContent_MakeAvailable(alglib)

# Add the executable
add_executable(main main.cpp)

# If ALGLIB is header-only or doesn't provide a library target:
target_include_directories(main PRIVATE ${alglib_SOURCE_DIR}/src)

# If ALGLIB provides a CMake target (for example, alglib::alglib), link it
# Uncomment the line below if a proper CMake target is available
# target_link_libraries(main PRIVATE alglib::alglib)

# Manually add the source files if ALGLIB doesn't provide a CMake target
file(GLOB ALGLIB_SRC 
    ${alglib_SOURCE_DIR}/src/*.cpp
)

# Link the source files to your project if ALGLIB doesn't have a CMake target
target_sources(main PRIVATE ${ALGLIB_SRC})

# Message to confirm the source directory of ALGLIB
message(STATUS "ALGLIB source dir: ${alglib_SOURCE_DIR}")

