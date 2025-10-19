#pragma once
#include <vector>
#include <iostream>
#include <limits>

namespace util {

template<typename T>
void get_cache_arguments(std::size_t& cache_size, std::vector<T>& elements)
{
    size_t count;

    while (!(std::cin >> cache_size >> count))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Error: enter two valid integers: ";
    }

    elements.resize(count);

    for (size_t i = 0; i < count; ++i)
    {
        while (!(std::cin >> elements[i]))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Error: enter valid value #" << (i+1) << ": ";
        }
    }
}

} // namespace util
