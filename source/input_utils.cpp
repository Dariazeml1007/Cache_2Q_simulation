#include <iostream>
#include <limits>

#include "input_utils.hpp"

namespace util
{

void get_cache_arguments(std::size_t& cache_size, std::vector<int>& elements)
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
            std::cout << "Error: enter valid integer #" << (i+1) << ": ";
        }
    }
}

} // namespace utils
