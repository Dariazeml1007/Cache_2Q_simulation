cmake_minimum_required(VERSION 3.16)
project(CacheTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()


set(TESTS_DIR ${CMAKE_SOURCE_DIR})
set(CACHE_2Q_EXECUTABLE ${CMAKE_SOURCE_DIR}/../build/cache_2q)
set(CACHE_IDEAL_EXECUTABLE ${CMAKE_SOURCE_DIR}/../build/cache_ideal)

file(GLOB TEST_INPUTS "${TESTS_DIR}/tests_files/*.in")

foreach(test_file ${TEST_INPUTS})
    get_filename_component(name ${test_file} NAME_WE)

    add_test(
        NAME twoq_${name}
        COMMAND ${TESTS_DIR}/verify.sh
                ${CACHE_2Q_EXECUTABLE}
                ${test_file}
                ${TESTS_DIR}/tests_files/${name}_2q.ref
    )

    add_test(
        NAME ideal_${name}
        COMMAND ${TESTS_DIR}/verify.sh
                ${CACHE_IDEAL_EXECUTABLE}
                ${test_file}
                ${TESTS_DIR}/tests_files/${name}_id.ref
    )
endforeach()
