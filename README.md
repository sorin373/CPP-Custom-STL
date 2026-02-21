# CPP-CUSTOM-STL

A C++17 reimplementation of selected Standard Template Library (STL) containers, developed as a systems-level exploration of container internals, allocator-aware design, memory safety, and performance behavior.

This project focuses on correctness, explicit lifetime management, and measurable performance characteristics rather than full standard conformance.

---

# Motivation

The C++ Standard Library provides high-performance, production-grade containers.  
However, its internal mechanisms remain opaque.

This project was developed to:

- Understand allocator-aware container architecture
- Implement iterator and ownership semantics from scratch
- Explore load factor and hashing behavior
- Study memory layout and cache implications
- Benchmark against `std` implementations
- Detect and eliminate undefined behavior using sanitizers

The goal is not replacement, but reconstruction for deep technical understanding.

---

# Implemented Containers

- `vector`
- `unordered_map`
- `forward_list`
- `array`

All containers are templated and implemented with clear separation between interface (`.h`) and implementation (`.tcc`).

---

# Design Philosophy

## 1. Allocator Awareness

All dynamic containers use `std::allocator_traits` and proper rebinding to:

- Allocate nodes
- Allocate bucket arrays (`unordered_map`)
- Construct and destroy objects correctly
- Preserve allocator state in node extraction

## 2. Explicit Object Lifetime Management

Objects are:

- Constructed using placement new or allocator_traits::construct
- Destroyed explicitly via allocator_traits::destroy
- Deallocated using matching allocator state

No reliance on implicit behavior.

## 3. Clear Ownership Model

Each container:

- Owns its storage
- Guarantees destruction of contained elements
- Defines valid moved-from states
- Avoids double frees and dangling pointers

## 4. Controlled Rehashing and Growth

`unordered_map`:

- Uses separate chaining
- Configurable load factor
- Explicit `rehash()` and `reserve()` logic
- Bucket redistribution without element corruption

`vector`:

- Manual growth strategy
- Move-aware reallocation
- Capacity preserved unless explicitly modified

## 5. Benchmark-Driven Validation

Performance was measured against `std` implementations under identical conditions.

Sanitizers were used to validate absence of memory errors and undefined behavior.

---

# Build System

Modern CMake configuration with:

- Out-of-source build enforcement
- Optional test targets
- Optional benchmark targets
- Optional ASAN / UBSAN integration

## Basic Build

```bash
cmake -S . -B build
cmake --build build
```

## Enable Tests

```bash
cmake -S . -B build -DSTL_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build
```

## Enable Sanitizers

```bash
cmake -S . -B build -DSTL_ENABLE_ASAN=ON -DSTL_ENABLE_UBSAN=ON
cmake --build build
```

---

# Benchmark Results

Test Parameters:

```
N = 200000
Q = 20000
E = 100000
```

All benchmarks compiled with identical compiler flags and executed in the same environment.

---

| Operation                              | std::vector | stl::vector |
|----------------------------------------|------------|------------|
| push_back copy (no reserve)           | 61 ms      | 47 ms      |
| push_back copy (reserve)              | 24 ms      | 23 ms      |
| push_back move (reserve)              | 32 ms      | 25 ms      |
| emplace_back (reserve)                | 27 ms      | 21 ms      |
| resize grow + shrink                  | 8 ms       | 11 ms      |
| clear + refill                        | 20 ms      | 20 ms      |
| insert middle                         | 182 ms     | 404 ms     |
| erase middle                          | 186 ms     | 152 ms     |

### Analysis

Append-heavy workloads perform competitively or faster than `std`.  
Middle insertion remains the most expensive operation due to element shifting cost.

---

| Operation                        | std::unordered_map | stl::unordered_map |
|----------------------------------|-------------------|-------------------|
| insert (no reserve)             | 407 ms            | 477 ms            |
| insert (reserve)                | 305 ms            | 358 ms            |
| operator[] insert               | 305 ms            | 370 ms            |
| find HIT                        | 218 ms            | 197 ms            |
| find MISS                       | 271 ms            | 186 ms            |
| erase by key                    | 192 ms            | 154 ms            |

### Analysis

Insertion is slower than `std`, but lookup (especially miss cases) and erase operations outperform the standard implementation in this workload.

This indicates efficient bucket traversal and node unlinking.

---

| Operation                     | std::forward_list | stl::forward_list |
|-------------------------------|------------------|------------------|
| push_front                   | 24 ms            | 24 ms            |
| insert_after (before_begin)  | 24 ms            | 25 ms            |
| iterate / sum                | 29 ms            | 30 ms            |
| linear_find HIT              | 19962 ms         | 20761 ms         |
| linear_find MISS             | 45437 ms         | 47227 ms         |
| pop_front (E times)          | 22 ms            | 23 ms            |

### Analysis

Core operations match `std` behavior closely.  
Linear search overhead aligns with expected O(n) traversal cost.

---

# Example Usage

```cpp
#include "unordered_map.h"

int main() {
    stl::unordered_map<int, int> map;

    map.insert({1, 42});
    map.insert({2, 84});

    if (map.contains(1)) {
        return map.at(1);
    }
}
```

---

# Technical Highlights

- Manual rehash implementation with correct bucket redistribution
- Single-pass insertion without double lookup
- Proper node extraction preserving allocator state
- Iterator and const_iterator support
- Explicit move constructor and move assignment handling
- Verified under AddressSanitizer and UndefinedBehaviorSanitizer

---

# Scope

This project prioritizes:

- Clarity of design
- Correct memory management
- Measurable performance
- Systems-level understanding

It does not aim for full ISO standard compliance.

---

# Requirements

- CMake 3.16+
- C++17 compatible compiler (GCC / Clang / MSVC)

---

# License

MIT License