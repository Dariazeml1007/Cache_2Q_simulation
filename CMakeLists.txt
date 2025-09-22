cmake_minimum_required(VERSION 3.10)
project(2Q_Cache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Основная программа с 2Q кэшем
add_executable(cache_2q source/cache_2q.cpp)
target_include_directories(cache_2q PRIVATE include)

# Программа с идеальным кэшем
add_executable(cache_ideal source/cache_ideal.cpp)
target_include_directories(cache_ideal PRIVATE include)

# Общие настройки для обоих
foreach(target cache_2q cache_ideal)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE -Wall -Wextra -Werror)
    endif()

    # Опционально: режим пользовательского ввода
    if(RUN_USER_INPUT)
        target_compile_definitions(${target} PRIVATE RUN_USER_INPUT)
    endif()
endforeach()
