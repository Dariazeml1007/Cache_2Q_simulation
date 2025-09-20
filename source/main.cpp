#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <unordered_map>

#include "cache_2q.hpp"
#include "tests_2q.hpp"

void  get_cache_arguments(size_t* cache_size, std::vector<int>* elements);

void  get_cache_arguments(size_t* cache_size, std::vector<int>* elements)
{
    size_t amount_of_elems;
    std::cin >> *cache_size >> amount_of_elems; // no need to use type-specifiers!!!

    elements->resize(amount_of_elems); // wow, finally a safe extension...

    for (size_t i = 0; i < amount_of_elems; i++)
    {
        std::cin >> (*elements)[i];
    }
}

int main()
{
    //size_t cache_size;
    //std::vector<int> elements;
//
   // get_cache_arguments(&cache_size, &elements);
    //std::cout << cache_size << '\n';

    //CacheBase cache(cache_size);
     run_all_tests();

    //for (int key : elements)
    //{
    //    cache.get(key);
    //}

    //std::cout << cache.hits << "\n";

    return 0;
}
