cmake_minimum_required(VERSION 3.16)
project(CacheSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(cache_2q source/cache_2q.cpp)
add_executable(cache_ideal source/cache_ideal.cpp)

target_include_directories(cache_2q PRIVATE include)
target_include_directories(cache_ideal PRIVATE include)
