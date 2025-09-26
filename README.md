# 2Q Cache Simulator

A C++ implementation of the **2Q (Two-Queue)** cache replacement algorithm.

The 2Q algorithm is an enhanced alternative to LRU that separates items into "one-time" and "frequently used" categories.

---

## 🔧 Features

- Implementation of the classic **2Q algorithm**:
  - `A1in`: FIFO queue for new (one-time) accesses.
  - `Am`: LRU queue for "hot", frequently used items.
- Implementation of the `A_out` metadata list for dropped items `A1in`.
- Fast O(1) average access using a hash table (`std::unordered_map`).
- Cache size control: `size_a1in = N/2`, `size_am = N/2`.
- Hit count tracking (`hits`) for efficiency evaluation.
- Built-in testing system.
## Output & Input
Input:
On stdin, first read the cache size and the number of elements, followed by the elements themselves (as integers).
Output:
On stdout, print the number of cache hits.

Example:
Input: 2 6 1 2 1 2 1 2
Output: 4

## 🛠️ Build and Run
 1. Создайте папку для сборки
  ```c
mkdir build
cd build
  ```
 2. Сгенерируйте файлы сборки
  ```c
cmake ..
  ```
 3. Соберите проект
  ```c
make
  ```
4. Запустите 2Q или Ideal
  ```c
./cache_2q

./cache_ideal
  ```
##  Run Built-in Tests
To run automated tests instead:
1. Open `cache_2q.cpp`.
2. Comment the line: `#define RUN_USER_INPUT_2Q`
3. Recompile and run.

## Comparison with ideal cache


| №  | Cache_size | Sequence                                | 2Q Hits | Ideal Hits |
|----|-------------|-----------------------------------------------------|---------|------------|
| 1  | 4           | `1,2,3,1,4,2,5`                                   | 0       | 2          |
| 2  | 6           | `1,2,3,1,2,3,4,5,6`                               | 3       | 3          |
| 3  | 4           | `1,2,1,2,1,2,1,2`                                 | 6       | 6          |
| 4  | 6           | `1,2,3,4,4,5,2,3,3,7,8,9,1,2,3`                   | 6       | 7          |
| 5  | 8           | `1,2,3,4,5,6,3,5,8,9,6,10,5,3,4,2`                | 6       | 7          |


## 💡 Personal Notes

- The comments reflect my personal transition as a programmer from C to C++.
