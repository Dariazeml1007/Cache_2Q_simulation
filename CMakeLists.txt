cmake_minimum_required(VERSION 3.16)
project(CacheSimulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_EXECUTABLE_SUFFIX "")


add_executable(cache_2q source/cache_2q.cpp source/input_utils.cpp)
add_executable(cache_ideal source/cache_ideal.cpp source/input_utils.cpp)


target_include_directories(cache_2q PRIVATE include)
target_include_directories(cache_ideal PRIVATE include)

# === Tests ===
enable_testing()


set(TESTS_DIR ${CMAKE_SOURCE_DIR}/tests)


file(GLOB TEST_INPUTS "${TESTS_DIR}/tests_files/*.in")

foreach(test_file ${TEST_INPUTS})
    get_filename_component(name ${test_file} NAME_WE)


    add_test(
        NAME twoq_${name}
        COMMAND ${TESTS_DIR}/verify.sh
                $<TARGET_FILE:cache_2q>
                ${test_file}
                ${TESTS_DIR}/tests_files/${name}_2q.ref
    )

    add_test(
        NAME ideal_${name}
        COMMAND ${TESTS_DIR}/verify.sh
                $<TARGET_FILE:cache_ideal>
                ${test_file}
                ${TESTS_DIR}/tests_files/${name}_id.ref
    )
endforeach()
