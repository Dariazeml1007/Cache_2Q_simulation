cmake_minimum_required(VERSION 3.16)
project(2Q_Cache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Всего две опции: по умолчанию USER_INPUT, если OFF - то TESTS
option(RUN_TESTS "Run automated tests instead of user input" OFF)

add_executable(cache_2q source/cache_2q.cpp)
add_executable(cache_ideal source/cache_ideal.cpp)

foreach(target cache_2q cache_ideal)
    target_include_directories(${target} PRIVATE include)

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE -Wall -Wextra -Werror)
    endif()

    if(RUN_TESTS)
        target_compile_definitions(${target} PRIVATE RUN_TESTS)
        message(STATUS "${target}: TEST mode")
    else()
        message(STATUS "${target}: USER INPUT mode")
    endif()
endforeach()
