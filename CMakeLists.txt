#Set your CMake Minimum requirement
cmake_minimum_required(VERSION 3.2.1)

#Define your project
project (Project1)

#Find required packages
find_package(OpenCV REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)

# Add project cmake modules to path.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)
set(NAME_SRC app/main.cpp app/Files.cpp app/Cleaner.cpp app/Thresholder.cpp)
set(NAME_HEADERS include/Files.hpp include/Cleaner.hpp include/Thresholder.hpp)

# We probably don't want this to run on every build.
option(COVERAGE "Generate Coverage Data" OFF)

if (COVERAGE)
    include(CodeCoverage)
    set(LCOV_REMOVE_EXTRA "'vendor/*'")
    setup_target_for_coverage(code_coverage test/cpp-test coverage)
    set(COVERAGE_SRCS app/main.cpp app/Files.cpp app/Cleaner.cpp app/Thresholder.cpp include/Files.hpp include/Cleaner.hpp include/Thresholder.hpp)

    SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
    SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
    SET(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
else()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -g")
endif()

include(CMakeToolsHelpers OPTIONAL)
include_directories(${CMAKE_SOURCE_DIR}/include)

#Add sub-directories
add_subdirectory(app)
add_subdirectory(test)
add_subdirectory(vendor/googletest/googletest)

#Add executables
add_executable(Project1 ${NAME_SRC} ${NAME_HEADERS})

#Link libraries
target_link_libraries(Project1 ${OpenCV_LIBS} ${Boost_LIBRARIES})
