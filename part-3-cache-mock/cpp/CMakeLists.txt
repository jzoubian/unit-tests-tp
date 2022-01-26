######################################################
cmake_minimum_required(VERSION 3.2)
project(tp-test-particle-cpp VERSION 0.0.0 LANGUAGES CXX)

######################################################
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

######################################################
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})

######################################################
enable_testing()

######################################################
add_library(cache SHARED range.cpp cache.cpp)

######################################################
add_executable(test-range test-range.cpp)
target_link_libraries(test-range cache GTest::GTest GTest::Main)
add_test(test-range test-range)

######################################################
add_executable(test-cache test-cache.cpp)
target_link_libraries(test-cache cache GTest::GTest gmock gmock_main)
add_test(test-cache test-cache)

######################################################
add_executable(test-cache-manual test-cache-manual.cpp)
target_link_libraries(test-cache-manual cache GTest::GTest GTest::Main)
add_test(test-cache-manual test-cache-manual)

######################################################
add_executable(test-basic-mock test-basic-mock.cpp)
target_link_libraries(test-basic-mock GTest::GTest gmock gmock_main)
add_test(test-basic-mock test-basic-mock)
