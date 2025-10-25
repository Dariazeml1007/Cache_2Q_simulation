#pragma once
#include <vector>
#include <iostream>
#include <limits>

namespace util
{
template<typename T>
void get_cache_arguments(std::size_t& cache_size, std::vector<T>& elements)
{
    size_t count;

    if (!(std::cin >> cache_size >> count))
    {
        throw std::runtime_error("Invalid input format: expected cache_size and element_count");
    }

    elements.resize(count);

    for (auto& element : elements)
    {
        if (!(std::cin >> element))
        {
            throw std::runtime_error("Invalid element value");
        }
    }
}

} // namespace util
