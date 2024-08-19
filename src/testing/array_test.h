#ifndef __ARRAY_TEST_H__
#define __ARRAY_TEST_H__

#include <iostream>
#include <cassert>
#include "../STL/array.h"

unsigned int COUNT = 0;

#define TEST_CASE(x) { if (!(x)) std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; else { std::cout << "Test case on line " << __LINE__ << " passed successfully!\n"; ++COUNT; } }

template <typename T, unsigned int ARRAY_SIZE>
class array_container_test
{
public:
    void __TEST__()
    {
        COUNT = 0;

        TEST_CASE(test_0());
        TEST_CASE(test_1(2, arr[2]));
        TEST_CASE(test_2(2, arr[2]));
        TEST_CASE(test_3(arr[0]));
        TEST_CASE(test_4(arr[ARRAY_SIZE - 1]));
        TEST_CASE(test_5());
        TEST_CASE(test_6());
        TEST_CASE(test_7());
        TEST_CASE(test_8());
        TEST_CASE(test_9());
        TEST_CASE(test_10());
        TEST_CASE(test_11(arr[3]));
        TEST_CASE(test_12());

        std::cout << "\n" << COUNT << "/" << N << " passed!\n\n";
    }

    array_container_test() { }

    array_container_test(const std::initializer_list<T>& init) : arr(init), il(init) { }

private:
    bool test_0()
    {
        stl::array<T, ARRAY_SIZE> expected = il;

        for (unsigned int i = 0; i < ARRAY_SIZE; ++i)
            if (arr[i] != expected[i])
                return false;

        return true;
    }

    bool test_1(const unsigned int& index, const T& res) { return arr[index] == res; }

    bool test_2(const unsigned int& index, const T& res) { return arr.at(index) == res; }

    bool test_3(const T& res) { return arr.front() == res; }

    bool test_4(const T& res) { return arr.back() == res; } 

    bool test_5() { return arr.max_size() == ARRAY_SIZE; }

    bool test_6() { return arr.size() == ARRAY_SIZE; }

    bool test_7()
    {
        stl::array<T, ARRAY_SIZE> aux;
        aux = arr;

        for (unsigned int i = 0; i < ARRAY_SIZE; ++i)
            if (arr[i] != aux[i])
                return false;
        
        return true;
    }

    bool test_8()
    {
        stl::array<T, ARRAY_SIZE> aux;
        aux = il;

        for (unsigned int i = 0; i < ARRAY_SIZE; ++i)
            if (arr[i] != aux[i])
                return false;

        return true;
    }

    bool test_9()
    {
        stl::array<T, ARRAY_SIZE> expected = arr;

        const T* data = arr.data();

        for (unsigned int i = 0; i < ARRAY_SIZE; ++i)
            if (data[i] != expected[i])
                return false;

        return true;
    }

    bool test_10()
    {
        stl::array<T, ARRAY_SIZE> expected = arr;
        typename stl::array<T, ARRAY_SIZE>::iterator it = arr.begin();

        unsigned int index = 0;
        
        while (it != arr.end())
        {
            if (*it != arr[index])
                return false;

            ++index;    
            ++it;
        }

        return true;
    }

    bool test_11(const T& res) { return stl::get<3>(arr) == res; }

    bool test_12() 
    {
        stl::array<int, 3> a1 = {1, 2, 3};
        stl::array<int, 3> a2 = {1, 2, 3};
        stl::array<int, 3> a3 = {1, 2, 4};

        // Test Case 1: Comparing arrays of the same size
        if (a1 < a2 == true)   return false;
        if (a1 > a2 == true)   return false;
        if (a1 <= a2 == false) return false;
        if (a1 >= a2 == false) return false; 

        if (a1 < a3 == false)  return false;
        // if (!(a3 < a1)) return false;
        // if (!(a3 > a1)) return false;
        // if (!(a1 > a3)) return false;
        // if (!(a3 >= a1)) return false;
        // if (!(a1 >= a3)) return false;

        // Test Case 2: Different Sizes
        // stl::array<int, 2> b1 = {1, 2};     // Smaller array
        // stl::array<int, 3> b2 = {1, 2, 3};  // Larger array

        // if (!(b1 < b2)) return false;
        // if (!(b2 < b1)) return false;
        // if (!(b2 > b1)) return false;
        // if (!(b1 > b2)) return false;
        // if (!(b2 >= b1)) return false;
        // if (!(b1 >= b2)) return false;

        // // Test Case 3: Edge Cases
        // stl::array<int, 0> c1 = {};          // Empty array
        // stl::array<int, 3> c2 = {1, 2, 3};   // Non-empty array

        // if (!(c1 < c2)) return false;
        // if (!(c2 < c1)) return false;
        // if (!(c2 > c1)) return false;
        // if (!(c1 > c2)) return false;
        // if (!(c2 >= c1)) return false;
        // if (!(c1 >= c2)) return false;

        return true;
    }

    stl::array<T, ARRAY_SIZE>      arr;
    const std::initializer_list<T> il;

    constexpr static unsigned int N  = 13;
};

#endif