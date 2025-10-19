
#include <iostream>
#include "cache_2q.hpp"
#include "input_utils.hpp"

auto slow_get_page = [](int key) -> std::string
{
    return "data_for_key_" + std::to_string(key);
};

int main()
{
    try
    {
        size_t cache_size;
        std::vector<int> elements;

        util::get_cache_arguments(cache_size, elements);

        caches::Cache_2Q<int, std::string> cache(cache_size, slow_get_page);

        size_t hits = 0;
        for (int key : elements)
        {

            hits += cache.get(key).first;
        }

        std::cout << hits << "\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown exception!\n";
        return 1;
    }

    return 0;
}
