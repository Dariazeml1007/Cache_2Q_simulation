
#include <iostream>
#include "cache_ideal.hpp"
#include "input_utils.hpp"

int main()
{
    try
    {
        size_t cache_size;
        std::vector<int> elements;

        util::get_cache_arguments(cache_size, elements);

        caches::IdealCache<int> cache(cache_size, elements);
        size_t hits = cache.run();

        std::cout << hits << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown exception!\n";
        return 1;
    }

    return 0;
}
