# CPP-CUSTOM-STL

A C++17 reimplementation of selected Standard Template Library (STL) containers, focused on allocator awareness, iterator correctness, memory safety, and performance analysis.

This project is intended as a systems-level exploration of container internals rather than a replacement for `std`.

---

## Overview

CPP-CUSTOM-STL provides custom implementations of fundamental STL containers with emphasis on:

- Allocator-aware design using `allocator_traits`
- Correct object lifetime management
- Iterator and const-iterator semantics
- Explicit rehashing and load factor control
- Separation between interface (`.h`) and implementation (`.tcc`)
- Performance benchmarking against `std` equivalents

Currently implemented containers include:

- `unordered_map`
- `vector`
- `forward_list`
- `array`

---

## Design Principles

- Correctness first — well-defined ownership and lifetime rules
- Allocator compliance — support for custom allocators and rebinding
- Exception safety awareness
- Explicit memory management
- Minimal abstraction leakage

This repository is structured to make internal mechanisms transparent and inspectable.

---

## `unordered_map` Implementation

The `unordered_map` implementation includes:

- Separate chaining hash table
- Configurable load factor
- Automatic rehashing
- Bucket-level iterators
- Node extraction support
- Allocator rebinding for bucket storage
- Move semantics support
- Iterator and const-iterator compliance

Rehashing preserves element integrity while redistributing nodes across buckets.

---

## Build System

The project uses modern CMake with:

- Out-of-source build enforcement
- Optional benchmark and test targets
- Optional AddressSanitizer and UndefinedBehaviorSanitizer support

### Build

```bash
cmake -S . -B build
cmake --build build
```

### Enable Tests

```bash
cmake -S . -B build -DSTL_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build
```

### Enable Sanitizers

```bash
cmake -S . -B build -DSTL_ENABLE_ASAN=ON -DSTL_ENABLE_UBSAN=ON
cmake --build build
```

Sanitizers are recommended when modifying container internals.

---

## Example

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

## Benchmarking

Benchmark targets compare performance against the corresponding `std` containers.

Typical scenarios include:

- Sequential insertion
- Random insertion
- Lookup performance
- Load factor stress tests

This enables analysis of hashing behavior and bucket distribution under varying conditions.

---

## Requirements

- CMake 3.16+
- C++17 compatible compiler (GCC / Clang / MSVC)

---

## Scope

This project prioritizes clarity and correctness over full standard compliance.
It is designed for educational, research, and systems programming exploration.

---

## License

MIT License