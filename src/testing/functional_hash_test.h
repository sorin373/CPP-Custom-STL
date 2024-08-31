#ifndef __FUNCTIONAL_HASH_TEST_H__
#define __FUNCTIONAL_HASH_TEST_H__

#include "../functional_hash/hash.h"

#include <iostream>
#include <cassert>

void test_hash_naive()
{
    // Test boolean values
    stl::hash<bool> hash_bool;
    assert(hash_bool(true) == 1);
    assert(hash_bool(false) == 0);

    // Test character values
    stl::hash<char> hash_char;
    assert(hash_char('A') == 65);

    // Test integer values
    stl::hash<int> hash_int;
    assert(hash_int(-1) == static_cast<stl::size_t>(-1));

    // Test floating-point values
    stl::hash<float> hash_float;
    assert(hash_float(3.14f) != hash_float(2.71f));

    stl::hash<double> hash_double;
    assert(hash_double(3.14) != hash_double(2.71));

    // Test long double values
    stl::hash<long double> hash_long_double;
    assert(hash_long_double(3.14L) != hash_long_double(2.71L));

    // Test string literals
    stl::hash<const char*> hash_cstr;
    assert(hash_cstr("hello") != hash_cstr("world"));
    assert(hash_cstr("hello") == hash_cstr("hello"));

    std::cout << "\nAll assertions passed!\n\n";
}

#endif