# 2Q Cache Simulator

A C++ implementation of the **2Q (Two-Queue)** cache replacement algorithm.

The 2Q algorithm is an enhanced alternative to LRU that separates items into "one-time" and "frequently used" categories.

---

## 🔧 Features

- Implementation of the classic **2Q algorithm**:
  - `A1in`: FIFO queue for new (one-time) accesses.
  - `Am`: LRU queue for "hot", frequently used items.
- Fast O(1) average access using a hash table (`std::unordered_map`).
- Cache size control: `size_a1in = N/2`, `size_am = N/2`.
- Hit count tracking (`hits`) for efficiency evaluation.
- Built-in testing system.

## 🛠️ Build and Run
 1. Создайте папку для сборки
  ```c
mkdir build
cd build
  ```
 3. Сгенерируйте файлы сборки
  ```c
cmake ..
  ```
 5. Соберите проект
  ```c
make
  ```
6. Запустите
  ```c
./cache_2q
  ```
## Comparison with ideal cache

-

## 💡 Personal Notes

- The comments reflect my personal transition as a programmer from C to C++.
