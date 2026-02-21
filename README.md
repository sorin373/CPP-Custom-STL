![C++ Standard](https://img.shields.io/badge/Standard-C%2B%2B17-blue.svg?style=flat&logo=c%2B%2B)
![License](https://img.shields.io/badge/License-MIT-green.svg?style=flat)
![Build](https://img.shields.io/badge/Build-CMake-orange.svg?style=flat&logo=cmake)

**A C++17 reimplementation of selected Standard Template Library (STL) containers.**

This project is a systems-level exploration of container internals, allocator-aware design, explicit memory safety, and performance behavior. It prioritizes **correctness**, **explicit lifetime management**, and **measurable performance** over full ISO standard conformance.

---

## 🎯 Motivation

The C++ Standard Library provides production-grade containers, but its internal mechanisms are often opaque. This project reconstructs them to achieve deep technical understanding in the following areas:

* **Architecture:** Understanding allocator-aware container design.
* **Memory:** Implementing manual iterator and ownership semantics.
* **Performance:** Exploring load factors, hashing behavior, and cache implications.
* **Safety:** Detecting and eliminating undefined behavior using sanitizers.

> **Goal:** Not replacement, but reconstruction for educational and analytical purposes.

---

## 📦 Implemented Containers

All containers are templated with a clear separation between interface (`.h`) and implementation (`.tcc`).

| Container | Type | Key Features |
| :--- | :--- | :--- |
| **`vector`** | Dynamic Array | Manual growth strategy, move-aware reallocation. |
| **`unordered_map`** | Hash Map | Separate chaining, configurable load factor. |
| **`forward_list`** | Singly Linked List | Memory efficient, O(1) insertion/removal. |
| **`array`** | Static Array | Stack-allocated fixed-size buffer. |

---

## ⚙️ Design Philosophy

### 1. Allocator Awareness
I utilize `allocator_traits` for all dynamic memory operations to ensure:
* Proper node allocation and bucket array management.
* Preservation of allocator state during node extraction.
* Correct object construction/destruction via rebind.

### 2. Explicit Lifetime Management
No reliance on implicit behavior. Objects are:
* Constructed via placement new or `allocator_traits::construct`.
* Destroyed explicitly via `allocator_traits::destroy`.
* Deallocated using matching allocator states.

### 3. Safety & Ownership
* **Ownership:** Each container strictly owns its storage.
* **Hygiene:** Guarantees destruction of elements; avoids double frees and dangling pointers.
* **States:** Defines valid "moved-from" states.

### 4. Controlled Growth
* **`unordered_map`:** Explicit `rehash()`/`reserve()` logic prevents element corruption during bucket redistribution.
* **`vector`:** Capacity is preserved unless explicitly modified; growth strategy is manual and move-aware.

---

## 📊 Benchmark Results

Performance was measured against `std` implementations under identical conditions.
* **N** = 200,000 (Elements)
* **Q** = 20,000 (Queries)
* **E** = 100,000 (Erasures)

### 1. Vector Performance

| Operation | `std::vector` | `stl::vector` | Delta |
| :--- | :--- | :--- | :--- |
| **push_back copy** (no reserve) | 61 ms | **47 ms** |  -14ms |
| **push_back copy** (reserve) | 24 ms | **23 ms** | -1ms |
| **push_back move** (reserve) | 32 ms | **25 ms** | -7ms |
| **emplace_back** (reserve) | 27 ms | **21 ms** |  -6ms |
| **resize** (grow + shrink) | **8 ms** | 11 ms | +3ms |
| **clear + refill** | 20 ms | 20 ms | = |
| **insert middle** | **182 ms** | 404 ms | +222ms |
| **erase middle** | 186 ms | **152 ms** |  -34ms |

> **Analysis:** Append-heavy workloads perform competitively or faster than `std`. Middle insertion remains the most expensive operation due to element shifting costs.

### 2. Unordered Map Performance

| Operation | `std::unordered_map` | `stl::unordered_map` | Delta |
| :--- | :--- | :--- | :--- |
| **insert** (no reserve) | **407 ms** | 477 ms | +70ms |
| **insert** (reserve) | **305 ms** | 358 ms | +53ms |
| **operator[] insert** | **305 ms** | 370 ms | +65ms |
| **find HIT** | 218 ms | **197 ms** |  -21ms |
| **find MISS** | 271 ms | **186 ms** |  -85ms |
| **erase by key** | 192 ms | **154 ms** |  -38ms |

> **Analysis:** While insertion is slightly slower, lookup (especially miss cases) and erase operations outperform the standard implementation, indicating efficient bucket traversal and node unlinking.

### 3. Forward List Performance

| Operation | `std::forward_list` | `stl::forward_list` |
| :--- | :--- | :--- |
| **push_front** | 24 ms | 24 ms |
| **insert_after** | 24 ms | 25 ms |
| **iterate / sum** | 29 ms | 30 ms |
| **linear_find** (HIT) | 19.9 s | 20.7 s |
| **linear_find** (MISS) | 45.4 s | 47.2 s |

> **Analysis:** Core operations match `std` behavior closely. Linear search overhead aligns with expected O(n) traversal costs.

---

## 🛠 Build System

The project uses modern CMake (3.16+) with out-of-source build enforcement.

### Basic Build

```bash
cmake -S . -B build
cmake --build build