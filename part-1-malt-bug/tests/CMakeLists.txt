set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")
add_executable(test-main test-main)
add_test("integration-test-func-stats" ${CMAKE_CURRENT_SOURCE_DIR}/test-stats.sh ${CMAKE_SOURCE_DIR})
