#ifndef TEST_BASE_HPP
#define TEST_BASE_HPP

#include "cache_2q.hpp"

void run_test(int test_num, size_t cache_size, const std::vector<int>& sequence, size_t expected_hits);
void run_all_tests();

#endif
