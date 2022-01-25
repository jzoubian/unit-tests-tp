######################################################
cmake_minimum_required(VERSION 3.2)
project(tp-test-particle-cpp VERSION 0.0.0 LANGUAGES CXX)

######################################################
find_package(GTest REQUIRED)
include_directories(GTEST_INCLUDE_DIRS)

######################################################
enable_testing()

######################################################
add_library(experiment SHARED particle.cpp physics.cpp)

######################################################
add_executable(test-particle test-particle.cpp)
target_link_libraries(test-particle experiment GTest::GTest GTest::Main)
add_test(test-particle test-particle)

######################################################
add_executable(test-physics test-physics.cpp)
target_link_libraries(test-physics experiment GTest::GTest GTest::Main)
add_test(test-physics test-physics)
