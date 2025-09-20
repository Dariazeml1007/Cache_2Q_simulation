#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <unordered_map>

#include "tests_2q.hpp"
#include "cache_2q.hpp"

void run_test(int test_num, size_t cache_size, const std::vector<int>& sequence, size_t expected_hits)
{
    CacheBase cache(cache_size);
    for (int key : sequence)
    {
        cache.get(key);
    }
    if (cache.hits == expected_hits)
    {
        std::cout << "Test " << test_num << " passed: got " << cache.hits << " hits\n";
    } else
    {
        std::cout << "Test " << test_num << " FAILED: expected " << expected_hits
                  << ", but got " << cache.hits << "\n";
    }
    std::cout << cache.size_a1in <<" "<< cache.size_am <<"\n";
}

void run_all_tests()
{
    std::cout << "=== Running 2Q Cache Tests ===\n\n";

    run_test(1, 4, {1,2,3,1,4,2,5}, 0);

    run_test(2, 6, {1,2,3,1,2,3,4,5,6}, 3);

    run_test(3, 4, {1,2,1,2,1,2,1,2}, 6);

    run_test(4, 6, {1,2,3,4,4,5,2,3,3,7,8,9,1,2,3}, 6);

    run_test(5, 8, {1,2,3,4,5,6,3,5,8,9,6,10,5,3,4,2}, 6);

    std::cout << "=== Tests completed ===\n";
}
