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

---

## 📦 Project Structure

- `cache_2q.hpp` — Main cache structure and method declarations.
- `cache_2q.cpp` — Method implementations (`get`, `putNew`, `putOld`).
- `main.cpp` — Entry point, data input, and cache execution.

---

## 💡 Personal Notes

- The comments reflect my personal transition as a programmer from C to C++.
