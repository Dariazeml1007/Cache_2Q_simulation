#include <iostream>
#include <vector>
#include "cache_ideal.hpp"

using namespace std;

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

void run_ideal_cache_tests()
{
    cout << "=== IDEAL CACHE TESTS ===" << endl;

    // Test case
    vector<int> requests = {1, 2, 3, 1, 4, 2, 5, 1, 3};
    IdealCache<int> ideal_cache(3, requests);

    size_t hits = ideal_cache.run();
    cout << "Ideal cache hits: " << hits << endl;
}

int main()
{
#ifdef RUN_USER_INPUT
    // Mode: user input
    size_t cache_size;
    vector<int> requests;

    get_cache_arguments(&cache_size, &requests);

    IdealCache<int> cache(cache_size, requests);
    size_t hits = cache.run();

    cout << "\n=== IDEAL CACHE RESULTS ===" << endl;
    cout << "Total requests: " << requests.size() << endl;
    cout << "Cache hits: " << hits << endl;
    cout << "Hit rate: " << (static_cast<double>(hits) / requests.size() * 100) << "%" << endl;

#else
    // Mode: tests
    run_ideal_cache_tests();
#endif

    return 0;
}
