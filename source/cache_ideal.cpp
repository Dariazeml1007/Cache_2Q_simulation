#include <iostream>
#include <vector>
#include "cache_ideal.hpp"

using namespace std;
// Comment for tests
 #define RUN_USER_INPUT_IDEAL

void get_cache_arguments(size_t* cache_size, vector<int>* elements)
{
    size_t count;
    cin >> *cache_size >> count;

    elements->resize(count);
    for (size_t i = 0; i < count; ++i)
    {
        cin >> (*elements)[i];
    }
}

void run_test_ideal(int test_num, size_t cache_size, const std::vector<int>& sequence, size_t expected_hits)
{
    IdealCache cache(cache_size, sequence );

    if (cache.run() == expected_hits)
    {
        std::cout << "Test " << test_num << " passed: got " << cache.hits() << " hits\n";
    }
    else
    {
        std::cerr << "Test " << test_num << " FAILED: expected " << expected_hits
                  << ", but got " << cache.hits() << "\n";
    }
}

void run_all_ideal_tests()
{
    std::cout << "=== Running Ideal Cache Tests ===\n\n";

    run_test_ideal(1, 4, {1,2,3,1,4,2,5}, 2);

    run_test_ideal(2, 6, {1,2,3,1,2,3,4,5,6}, 3);

    run_test_ideal(3, 4, {1,2,1,2,1,2,1,2}, 6);

    run_test_ideal(4, 6, {1,2,3,4,4,5,2,3,3,7,8,9,1,2,3}, 7);

    run_test_ideal(5, 8, {1,2,3,4,5,6,3,5,8,9,6,10,5,3,4,2}, 7);

    std::cout << "=== Tests completed ===\n";
}

int main()
{
#ifdef RUN_USER_INPUT_IDEAL
    // Mode: user input
    size_t cache_size;
    vector<int> requests;

    get_cache_arguments(&cache_size, &requests);

    IdealCache<int> cache(cache_size, requests);
    size_t hits = cache.run();

    cout << "Cache hits: " << hits << endl;

#else
    // Mode: tests
    run_all_ideal_tests();
#endif

    return 0;
}
