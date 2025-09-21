cmake_minimum_required(VERSION 3.10)
project(2Q_Cache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Просто добавляем все .cpp файлы из папки source
file(GLOB SOURCES "source/*.cpp")

add_executable(cache_2q ${SOURCES})

# Указываем папку include для заголовков
target_include_directories(cache_2q PRIVATE include)

# Базовые флаги для GCC/Clang
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(cache_2q PRIVATE -Wall -Wextra -Werror)
endif()
