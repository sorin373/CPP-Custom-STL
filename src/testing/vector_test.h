#ifndef __VECTOR_TEST_H__
#define __VECTOR_TEST_H__

#include "../STL/allocator.h"
#include "../STL/vector.h"

#include <iostream>

#define TEST_CASE(x) { if (!(x)) std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; else { std::cout << "Test case on line " << __LINE__ << " passed successfully!\n"; ++COUNT; } }

template <
    typename T, 
    typename Allocator = stl::allocator<T>
> class vector_container_test
{
public:
    void __TEST__()
    {
        COUNT = 0;

        TEST_CASE(test_0());

        std::cout << "\n" << COUNT << "/" << N << " passed!\n\n";
    }

    vector_container_test(const std::initializer_list<T>& init) : v(init), il(init) { }

private:

    bool test_0()
    {
        stl::vector<T, Allocator> expected = il;

        if (v.size() != expected.size())
            return false;

        for (unsigned int i = 0, size = v.size(); i < size; ++i)
            if (v[i] != expected[i])
                return false;

        return true;
    }


    stl::vector<T, Allocator>      v;
    const std::initializer_list<T> il;

    constexpr static unsigned int N = 1;
};

#endif // VECTOR_TEST_H