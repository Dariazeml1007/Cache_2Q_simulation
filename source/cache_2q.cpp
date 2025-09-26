#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <unordered_map>

#include "cache_2q.hpp"

// Comment for tests
#define RUN_USER_INPUT

void get_cache_arguments(size_t* cache_size, std::vector<int>* elements);
void run_test(int test_num, size_t cache_size, const std::vector<int>& sequence, size_t expected_hits);
void run_all_tests();

auto slow_get_page = [](int key) -> std::string
{
    return "data_for_key_" + std::to_string(key);
};

int main()
{
#ifdef RUN_USER_INPUT
    // Mode: keyboard input
    size_t cache_size;
    std::vector<int> elements;

    get_cache_arguments(&cache_size, &elements);

    // Create ideal cache with the sequence
    CacheBase<int, std::string> cache(cache_size, slow_get_page);
    size_t hits = 0;
    for (int key : elements)
    {
        hits += cache.get(key);
    }

    std::cout << hits << "\n";
#else
    // Mode: tests
    run_all_tests();
#endif

    return 0;
}

void get_cache_arguments(size_t* cache_size, std::vector<int>* elements)
{
    size_t amount_of_elems;
    std::cin >> *cache_size >> amount_of_elems; // no need to use type-specifiers!!!

    elements->resize(amount_of_elems); // wow, finally a safe extension...

    for (size_t i = 0; i < amount_of_elems; i++)
    {
        std::cin >> (*elements)[i];
    }
}

void run_test(int test_num, size_t cache_size, const std::vector<int>& sequence, size_t expected_hits)
{
    CacheBase<int, std::string> cache(cache_size, slow_get_page);

    size_t hits = 0;
    for (int key : sequence)
    {
        hits += cache.get(key);
    }

    if (hits == expected_hits)
    {
        std::cout << "Test " << test_num << " passed: got " << hits<< " hits\n";
    }
    else
    {
        std::cerr << "Test " << test_num << " FAILED: expected " << expected_hits
                  << ", but got " << hits << "\n";
    }
}

void run_all_tests()
{
    std::cout << "=== Running 2 Cache Tests ===\n\n";

    run_test(1, 4, {1,2,3,1,4,2,5}, 0);

    run_test(2, 6, {1,2,3,1,2,3,4,5,6}, 3);

    run_test(3, 4, {1,2,1,2,1,2,1,2}, 6);

    run_test(4, 6, {1,2,3,4,4,5,2,3,3,7,8,9,1,2,3}, 6);

    run_test(5, 8, {1,2,3,4,5,6,3,5,8,9,6,10,5,3,4,2}, 6);

    std::cout << "=== Tests completed ===\n";
}
