#ifndef __ARRAY_TEST_H__
#define __ARRAY_TEST_H__

#include "../STL/containers/array/array.h"

#include <iostream>
#include <cassert>

static unsigned int COUNT = 0;

#define TEST_CASE(C) { if (!(C)) std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; else { std::cout << "Test case on line " << __LINE__ << " passed successfully!\n"; ++COUNT; } }
#define __check_result__(X, Y)           { assert(X == Y); return true; }
#define __check_result_no_return__(X, Y) { assert(X == Y); }

template <
    typename T,
    stl::size_t ARRAY_SIZE
> class array_container_test
{
public:
    void __TEST__()
    {
        COUNT = 0;

        TEST_CASE(test_0());

        try
        {
            TEST_CASE(test_1(2, m_array[2]));
        }
        catch(const std::out_of_range&) { ++COUNT; }
        
        try
        {
            TEST_CASE(test_2(2, m_array[2]));
        }
        catch(const std::out_of_range&) { ++COUNT; }
        
        try
        {
            TEST_CASE(test_3(m_array[0]));
        }
        catch(const std::out_of_range&) { ++COUNT; }
        
        try
        {
            TEST_CASE(test_4(m_array[ARRAY_SIZE - 1]));
        }
        catch(const std::out_of_range&) { ++COUNT; }
        
        TEST_CASE(test_5());
        TEST_CASE(test_6());
        TEST_CASE(test_7());
        TEST_CASE(test_8());
        TEST_CASE(test_9());
        TEST_CASE(test_10());

        try
        {
            TEST_CASE(test_11(m_array[3]));
        }
        catch(const std::out_of_range&) { ++COUNT; }
        
        TEST_CASE(test_12());
        TEST_CASE(test_13());
        TEST_CASE(test_14());

        std::cout << "\n" << COUNT << "/" << N << " passed!\n\n";
    }

    constexpr array_container_test(const std::initializer_list<T>& init) noexcept : m_array(init), m_il(init) { }

private:
    bool test_0()
    {
        stl::array<T, ARRAY_SIZE> expected = m_array;
        
        for (stl::size_t i = 0; i < ARRAY_SIZE; ++i)
            __check_result_no_return__(m_array[i], expected[i]);

        return true;
    }

    bool test_1(const unsigned int& index, const T& res) 
    {
        try
        {
            T result = m_array[index];
            __check_result__(result, res);
        }
        catch(const std::out_of_range&) { }     

        return true;
    }

    bool test_2(const unsigned int& index, const T& res) 
    { 
        try
        {
            T result = m_array.at(index);
            __check_result__(result, res);
        }
        catch(const std::out_of_range&) { }

        return true;
    }

    bool test_3(const T& res) 
    {
        try
        {
            T result = m_array.front(); 
            __check_result__(result, res);
        }
        catch(const std::out_of_range&) { }

        return true;
    }

    bool test_4(const T& res) 
    { 
        try
        {
            T result = m_array.back();
            __check_result__(result, res);
        }
        catch(const std::out_of_range&) { }

        return true;
    } 

    bool test_5() 
    { 
        unsigned int result = m_array.size();
        __check_result__(result, ARRAY_SIZE);
    }

    bool test_6() 
    { 
        unsigned int result = m_array.max_size();
        __check_result__(result, ARRAY_SIZE);
    }

    bool test_7()
    {
        stl::array<T, ARRAY_SIZE> aux;
        aux = m_array;

        for (unsigned int i = 0; i < ARRAY_SIZE; ++i)
            __check_result_no_return__(m_array[i], aux[i]);
        
        return true;
    }

    bool test_8()
    {
        stl::array<T, ARRAY_SIZE> aux;
        aux = m_array;

        for (unsigned int i = 0; i < ARRAY_SIZE; ++i)
            __check_result_no_return__(m_array[i], aux[i]);

        return true;
    }

    bool test_9()
    {
        stl::array<T, ARRAY_SIZE> expected = m_array;

        const T* data = m_array.data();

        for (unsigned int i = 0; i < ARRAY_SIZE; ++i)
            __check_result_no_return__(data[i], expected[i]);

        return true;
    }

    bool test_10()
    {
        stl::array<T, ARRAY_SIZE> expected = m_array;
        typename stl::array<T, ARRAY_SIZE>::iterator it = m_array.begin();

        unsigned int index = 0;
        
        while (it != m_array.end()) {
            __check_result_no_return__(*it, m_array[index]);

            ++index;    
            ++it;
        }

        return true;
    }

    bool test_11(const T& res) 
    { 
        try
        {
            T result = stl::get<3>(m_array);
            __check_result__(result, res);
        }
        catch(const std::out_of_range&) { }
        
        return true;
    }

    bool test_12() 
    {
        stl::array<int, 3> a1 = {1, 2, 3}, a2 = {1, 2, 3}, a3 = {1, 2, 4};

        assert((a1 == a2)  && "a1 should be equal to a2");
        assert(!(a1 != a2) && "a1 should not be not equal to a2");
        assert(!(a1 == a3) && "a1 should not be equal to a3");

        assert(!(a1 < a2) && "a1 should not be less than a2");
        assert(!(a1 > a2) && "a1 should not be greater than a2");
        assert((a1 <= a2) && "a1 should be less than or equal to a2");
        assert((a1 >= a2) && "a1 should be greater than or equal to a2");

        assert((a1 < a3)   && "a1 should be less than a3");
        assert(!(a3 < a1)  && "a3 should not be less than a1");
        assert((a3 > a1)   && "a3 should be greater than a1");
        assert(!(a1 > a3)  && "a1 should not be greater than a3");
        assert((a3 >= a1)  && "a3 should be greater than or equal to a1");
        assert(!(a1 >= a3) && "a1 should not be greater than or equal to a3");

        return true;
    }

    bool test_13()
    {
        bool exception_thrown = false;

        try
        {
            m_array.at(m_array.size());
        }
        catch(const std::out_of_range&)
        {
            exception_thrown = true;
        }

        __check_result__(exception_thrown, true);
    }

    bool test_14()
    {
        stl::array<int, ARRAY_SIZE> test_array_only_int;

        test_array_only_int.fill(5);

        try
        {
            __check_result_no_return__(test_array_only_int[0], 5);
        }
        catch(const std::out_of_range&) { }

        try 
        {
            test_array_only_int[0] = 10;
            __check_result_no_return__(test_array_only_int[0], 10);
        }
        catch(const std::out_of_range&) { }

        return true;
    }

    stl::array<T, ARRAY_SIZE>      m_array;
    const std::initializer_list<T> m_il;   // this is used to reinitalize the array and to keep a copy of the original items

    constexpr static unsigned int N = 15;
};

#endif // ARRAY_TEST_H