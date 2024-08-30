/**
 *  @copyright MIT License

    Copyright (c) 2024 Sorin Tudose

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

    @file  vector_test.h

    @brief This file tests the API functionalities implemented inside the @c %vector container
           The tests include:
            
                \ 3 types of custom allocators wraping over the main allocator ( @c new, @c malloc and a tracking allocator), aka @c allocator ( See @file @c allocator.h ) \
                \ constructors \
                \ element access functions \
                \ iterators \
                \ capacity functions \
    
    @note See @file @c unit_testing.h to see all testing data types.
 */

#ifndef __VECTOR_TEST_H__
#define __VECTOR_TEST_H__

#include "../STL/vector.h"

#include <iostream>
#include <cassert>

#define TEST_CASE(C) { if (!(C)) std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; else { std::cout << "Test case on line " << __LINE__ << " passed successfully!\n"; ++COUNT; } }
#define __check_result_no_return__(X, Y) { assert(X == Y); }
#define __check_result__(X, Y) { assert(X == Y); return true; }
#define __send_console_msg__(VAR) { std::cerr << VAR << ' '; }
#define __debug_output_msg__ 0

#define __N_ALLOCS 10000

#define INT_MAX 2147483647
#define INT_MIN	(-INT_MAX-1)


        /****************************
        * CUSTOM TESTING ALLOCATORS *
        ****************************/

namespace my_alloc
{
    template <typename T>
    class custom_allocator_new
    {
    public:
        typedef T              value_type;
        typedef T*             pointer;
        typedef const T*       const_pointer;
        typedef stl::size_t    size_type; 
        typedef const T&       const_reference;

        custom_allocator_new() = default;
        ~custom_allocator_new() = default;

        pointer allocate(size_type size)
        {
            pointer ptr = static_cast<pointer>(::operator new(sizeof(T) * size));

            if (!ptr) throw std::bad_alloc();

            return ptr;
        }

        void deallocate(pointer p, size_type size) { ::operator delete(p); }

        size_type max_size() const { return std::numeric_limits<size_type>::max(); }

        void construct(pointer ptr, const_reference value) { new(static_cast<void*>(ptr)) T(value); }

        void destroy(pointer ptr) { ptr->~T(); }
    };

    template <typename T>
    class custom_allocator_malloc
    {
    public:
        typedef T              value_type;
        typedef T*             pointer;
        typedef const T*       const_pointer;
        typedef stl::size_t    size_type; 

        custom_allocator_malloc() = default;
        ~custom_allocator_malloc() = default;

        pointer allocate(size_type size)
        {
            pointer ptr = static_cast<pointer>(malloc(sizeof(T) * size));

            if (!ptr) throw std::runtime_error("malloc() failed!\n");

            return ptr;
        }

        void deallocate(pointer p, size_type size) { free(static_cast<void *>(p)); }

        size_type max_size() const { return std::numeric_limits<size_type>::max(); }
    };

    template <typename T>
    class custom_tracking_allocator
    {
    public:
        typedef T              value_type;
        typedef T*             pointer;
        typedef const T*       const_pointer;
        typedef stl::size_t    size_type; 
        typedef const T&       const_reference;

        constexpr custom_tracking_allocator() noexcept : m_allocs(0) { }
        ~custom_tracking_allocator() = default;

        pointer allocate(size_type size)
        {
            pointer ptr = static_cast<pointer>(malloc(sizeof(T) * size));

            if (!ptr) throw std::runtime_error("malloc() failed!\n");

            m_allocs += size;

            return ptr;
        }

        void deallocate(pointer p, size_type size) { free(static_cast<void *>(p)); }

        size_type max_size() const { return std::numeric_limits<size_type>::max(); }

        size_type get_allocs() const noexcept { return this->m_allocs; }

        void construct(pointer ptr, const_reference value) { new(static_cast<void*>(ptr)) T(value); }

        void destroy(pointer ptr) { ptr->~T(); }

    private:
        size_type m_allocs;
    };
}

        /******************************************************************************************************* */
        /*                                      GENERIC_TESTING_CONTAINER                                        */
        /******************************************************************************************************* */

template <
    typename T, 
    typename Allocator = my_alloc::custom_tracking_allocator<T>
> class vector_container_test
{
public:
    void __TEST__(const stl::size_t& __VECTOR_CAP = __N_ALLOCS)
    {
        COUNT = 0;

        TEST_CASE(test_0());
        TEST_CASE(test_1());
        TEST_CASE(test_2());
        TEST_CASE(test_3());
        TEST_CASE(test_4());
        TEST_CASE(test_5());
        TEST_CASE(test_6());
        TEST_CASE(test_7());
        TEST_CASE(test_8(__VECTOR_CAP));
        TEST_CASE(test_9(__VECTOR_CAP));
        TEST_CASE(test_10());
        TEST_CASE(test_11());
        TEST_CASE(test_12());
        TEST_CASE(test_13());
        TEST_CASE(test_14());
        TEST_CASE(test_15(__VECTOR_CAP));
        TEST_CASE(test_16());
        TEST_CASE(test_17());
        
        test_max_size();
        test_custom_allocator();

        std::cout << "\n" << COUNT << "/" << N << " passed!\n\n";
    }

    constexpr vector_container_test(const std::initializer_list<T>& init) noexcept : my_vector(init) { }

private:
    bool test_0()
    {
        stl::vector<T, Allocator> expected = my_vector;

        __check_result_no_return__(my_vector.size(), expected.size());

        for (stl::size_t i = 0, size = my_vector.size(); i < size; ++i)
            __check_result_no_return__(my_vector[i], expected[i]);

        return true;
    }

    bool test_1()
    {
        stl::vector<T, Allocator> expected = my_vector;

        __check_result_no_return__(my_vector.size(), expected.size());

        for (stl::size_t i = 0, size = my_vector.size(); i < size; ++i)
            __check_result_no_return__(my_vector[i], expected[i]);

        return true;
    }

    bool test_2()
    {
        stl::vector<T, Allocator> expected = my_vector;

        stl::size_t index = 0;
        for (auto it = my_vector.begin(), size = my_vector.size(); it != my_vector.end(); ++it)
        {
#if __debug_output_msg__
            __send_console_msg__(*it);
            __send_console_msg__(expected[index]);
            std::cout << "\n";
#endif   
            if (index == size && it != my_vector.end())
                return false;
            
            __check_result_no_return__(*it, expected[index]);
            
            ++index;
        }

        return true;
    }

    bool test_3()
    {
        stl::vector<T, Allocator> expected = my_vector;

        stl::size_t index = 0;
        for (auto cit = my_vector.cbegin(), size = my_vector.size(); cit != my_vector.cend(); ++cit)
        {
#if __debug_output_msg__
            __send_console_msg__(*cit);
            __send_console_msg__(expected[index]);
            std::cout << "\n";
#endif   
            if (index == size && cit != my_vector.cend())
                return false;
            
            __check_result_no_return__(*cit, expected[index]);
            
            ++index;
        }

        return true;
    }

    bool test_4()
    {
        stl::vector<T, Allocator> expected = my_vector;

        int index = my_vector.size() - 1;
        for (auto rit = my_vector.rbegin(); rit != my_vector.rend(); ++rit)
        {
#if __debug_output_msg__
            __send_console_msg__(*rit);
            __send_console_msg__(my_vector[index]);
            std::cout << "\n";
#endif   
            __check_result_no_return__(*rit, my_vector[index]);   
            --index;
        }

        return true;
    }

    bool test_5()
    {
        stl::vector<T, Allocator> expected = my_vector;

        int index = my_vector.size() - 1;
        for (auto crit = my_vector.crbegin(); crit != my_vector.crend(); ++crit)
        {
#if __debug_output_msg__
            __send_console_msg__(*crit);
            __send_console_msg__(my_vector[index]);
            std::cout << "\n";
#endif            
            if (*crit != my_vector[index])
                return false;
            
            --index;
        }

        return true;
    }

    bool test_6()
    {
        stl::vector<T, Allocator> expected = my_vector;
        stl::vector<T, Allocator> user_vector(my_vector.size());

        for (stl::size_t i = 0, size = my_vector.size(); i < size; ++i)
            user_vector[i] = my_vector[i];

#if __debug_output_msg__
        std::cout << "Container sizes:\n";

        __send_console_msg__(my_vector.size());
        std::cout << std::endl;

        __send_console_msg__(expected.size());
        std::cout << std::endl;

        __send_console_msg__(user_vector.size());
        std::cout << std::endl;
#endif
        __check_result_no_return__(expected.size(), user_vector.size());

        for (stl::size_t i = 0, size = user_vector.size(); i < size; ++i)
            __check_result_no_return__(user_vector[i], expected[i]);

        return true;
    }

    bool test_7()
    {
        stl::vector<T, Allocator> init = my_vector;
        stl::vector<T, Allocator> aux;

        for (stl::size_t i = 0, size = init.size(); i < size; ++i)
            aux.push_back(init[i]);

        __check_result_no_return__(aux.size(), init.size());

        for (stl::size_t i = 0, size = aux.size(); i < size; ++i)
            __check_result_no_return__(aux[i], init[i]);

        return true;
    }

    bool test_8(const stl::size_t& N = 0)
    {
        stl::vector<T, Allocator> expected;
        expected.assign(N, my_vector[0]);

        __check_result_no_return__(expected.size(), N);

        for (stl::size_t i = 0, size = expected.size(); i < size; ++i)
            __check_result_no_return__(expected[i], my_vector[0]);

        return true;
    }

    bool test_9(const stl::size_t& N = 0)
    {
        stl::vector<T, Allocator> expected(N, my_vector[0]);
        stl::vector<T, Allocator> new_vector;

        new_vector.assign(expected.begin(), expected.end());

        __check_result_no_return__(new_vector.size(), N);

        for (stl::size_t i = 0, size = new_vector.size(); i < size; ++i)
            __check_result_no_return__(expected[i], new_vector[i]);

        return true;
    }
    
    /** @fn fill() | standalone test */
    bool test_10()
    {
        stl::array<char, __N_ALLOCS> extra;
        extra.fill('b');

        stl::vector<char> characters;
        characters.assign(extra.begin(), extra.end());

        __check_result_no_return__(characters.size(), __N_ALLOCS);
        __check_result_no_return__(characters[0], 'b');
        __check_result_no_return__(characters[__N_ALLOCS - 1], 'b');

        return true;
    }

    /** @fn assign(std::initializer_list<Type>) | standalone test */
    bool test_11()
    {
        stl::vector<int> vec;
        vec.assign({INT_MAX, INT_MAX - 1, INT_MIN, INT_MIN + 1});

        stl::vector<int> expected;
        expected.assign({INT_MAX, INT_MAX - 1, INT_MIN, INT_MIN + 1});

        __check_result_no_return__(expected.size(), vec.size());

        for (stl::size_t i = 0, size = expected.size(); i < size; ++i)
            __check_result_no_return__(expected[i], vec[i]);

        return true;
    }

    bool test_12()
    {
        T lhs = my_vector.at(0), rhs = my_vector.at(my_vector.size() - 1);

        __check_result_no_return__(lhs, my_vector[0]);
        __check_result_no_return__(rhs, my_vector[my_vector.size() - 1]);

        return true;
    }

    bool test_13()
    {
        T lhs = my_vector.front(), rhs = my_vector.back();

        __check_result_no_return__(lhs, my_vector[0]);
        __check_result_no_return__(rhs, my_vector[my_vector.size() - 1]);

        return true;
    }

    bool test_14()
    {
        stl::vector<T, Allocator> expected = my_vector;
        
        __check_result_no_return__(my_vector.size(), expected.size());
        
        for (auto it = my_vector.begin(), expected_it = expected.begin(); it != my_vector.end(); ++it, ++expected_it)
            __check_result_no_return__(*it, *expected_it);
        
        return true;
    }

    /** @fn reserve() | standalone test */
    bool test_15(const stl::size_t& N = 0)
    {   
        stl::vector<int> vec;
        vec.reserve(N);

        __check_result_no_return__(vec.capacity(), N);

        vec.push_back(std::rand());
        vec.push_back(std::rand());

        // check that the memory was not reallocated when using push_back() | __N_ALLOCS >= vec.size()
        __check_result__(vec.capacity(), N);
    }
    
    bool test_16()
    {
        my_vector.shrink_to_fit();
        __check_result__(my_vector.capacity(), my_vector.size())
    }

    bool test_17()
    {
        stl::vector<T, Allocator> temp = my_vector;

        my_vector.clear();
        __check_result_no_return__(my_vector.size(), 0);

        my_vector = temp;

        return true;
    }
    
    /** @fn max_size() | standalone test */
    void test_max_size() { std::cout << "\nMax-size: " << std::uppercase << my_vector.max_size() << " | 0x" << std::hex << my_vector.max_size() << std::dec << std::endl; }

    /** @fn get_allocs() | standalone test */
    void test_custom_allocator() { std::cout << "Total size allocated: " << my_vector.get_allocator().get_allocs() << "\n"; }

    stl::vector<T, Allocator> my_vector;
    constexpr static stl::size_t N = 18;
};

#endif // VECTOR_TEST_H