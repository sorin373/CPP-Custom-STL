#ifndef __VECTOR_TEST_H__
#define __VECTOR_TEST_H__

#include "../STL/vector.h"

#include <iostream>
#include <cassert>

#include <vector>

#define TEST_CASE(C) { if (!(C)) std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; else { std::cout << "Test case on line " << __LINE__ << " passed successfully!\n"; ++COUNT; } }
#define __check_result_no_return__(X, Y) { assert(X == Y); }
#define __check_result__(X, Y) { assert(X == Y); return true; }
#define __send_console_msg__(VAR) { std::cerr << VAR << ' '; }
#define __debug_output_msg__ 0

#define __N_ALLOCS 10000

#define INT_MAX		2147483647
#define INT_MIN		(-INT_MAX-1)

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

template <
    typename T, 
    typename Allocator = my_alloc::custom_tracking_allocator<T>
> class vector_container_test
{
public:
    void __TEST__(const stl::size_t& SIZE = __N_ALLOCS)
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
        TEST_CASE(test_8(SIZE));
        TEST_CASE(test_9(SIZE));
        TEST_CASE(test_10());
        TEST_CASE(test_11());
        TEST_CASE(test_12());
        TEST_CASE(test_13());
        TEST_CASE(test_14());
        
        test_custom_allocator();

        std::cout << "\n" << COUNT << "/" << N << " passed!\n\n";
    }

    constexpr vector_container_test(const std::initializer_list<T>& init) noexcept : v(init), il(init) { }

private:
    bool test_0()
    {
        stl::vector<T, Allocator> expected = v;

        __check_result_no_return__(v.size(), expected.size());

        for (stl::size_t i = 0, size = v.size(); i < size; ++i)
            __check_result_no_return__(v[i], expected[i]);

        return true;
    }

    bool test_1()
    {
        stl::vector<T, Allocator> expected = v;

        __check_result_no_return__(v.size(), expected.size());

        for (stl::size_t i = 0, size = v.size(); i < size; ++i)
            __check_result_no_return__(v[i], expected[i]);

        return true;
    }

    bool test_2()
    {
        stl::vector<T, Allocator> expected = v;

        stl::size_t index = 0;
        for (auto it = v.begin(), size = v.size(); it != v.end(); ++it)
        {
#if __debug_output_msg__
            __send_console_msg__(*it);
            __send_console_msg__(expected[index]);
            std::cout << "\n";
#endif   
            if (index == size && it != v.end())
                return false;
            
            __check_result_no_return__(*it, expected[index]);
            
            ++index;
        }

        return true;
    }

    bool test_3()
    {
        stl::vector<T, Allocator> expected = v;

        stl::size_t index = 0;
        for (auto cit = v.cbegin(), size = v.size(); cit != v.cend(); ++cit)
        {
#if __debug_output_msg__
            __send_console_msg__(*cit);
            __send_console_msg__(expected[index]);
            std::cout << "\n";
#endif   
            if (index == size && cit != v.cend())
                return false;
            
            __check_result_no_return__(*cit, expected[index]);
            
            ++index;
        }

        return true;
    }

    bool test_4()
    {
        stl::vector<T, Allocator> expected = v;

        int index = v.size() - 1;
        for (auto rit = v.rbegin(); rit != v.rend(); ++rit)
        {
#if __debug_output_msg__
            __send_console_msg__(*rit);
            __send_console_msg__(v[index]);
            std::cout << "\n";
#endif   
            __check_result_no_return__(*rit, v[index]);   
            --index;
        }

        return true;
    }

    bool test_5()
    {
        stl::vector<T, Allocator> expected = v;

        int index = v.size() - 1;
        for (auto crit = v.crbegin(); crit != v.crend(); ++crit)
        {
#if __debug_output_msg__
            __send_console_msg__(*crit);
            __send_console_msg__(v[index]);
            std::cout << "\n";
#endif            
            if (*crit != v[index])
                return false;
            
            --index;
        }

        return true;
    }

    bool test_6()
    {
        stl::vector<T, Allocator> expected = v;
        stl::vector<T, Allocator> user_vector(v.size());

        for (stl::size_t i = 0, size = v.size(); i < size; ++i)
            user_vector[i] = v[i];

#if __debug_output_msg__
        std::cout << "Container sizes:\n";

        __send_console_msg__(v.size());
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
        stl::vector<T, Allocator> init = v;
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
        expected.assign(N, v[0]);

        __check_result_no_return__(expected.size(), N);

        for (stl::size_t i = 0, size = expected.size(); i < size; ++i)
            __check_result_no_return__(expected[i], v[0]);

        return true;
    }

    bool test_9(const stl::size_t& N = 0)
    {
        stl::vector<T, Allocator> expected(N, v[0]);
        stl::vector<T, Allocator> new_vector;

        new_vector.assign(expected.begin(), expected.end());

        __check_result_no_return__(new_vector.size(), N);

        for (stl::size_t i = 0, size = new_vector.size(); i < size; ++i)
            __check_result_no_return__(expected[i], new_vector[i]);

        return true;
    }

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
        T lhs = v.at(0), rhs = v.at(v.size() - 1);

        __check_result_no_return__(lhs, v[0]);
        __check_result_no_return__(rhs, v[v.size() - 1]);

        return true;
    }

    bool test_13()
    {
        T lhs = v.front(), rhs = v.back();

        __check_result_no_return__(lhs, v[0]);
        __check_result_no_return__(rhs, v[v.size() - 1]);

        return true;
    }

    bool test_14()
    {
        stl::vector<T, Allocator> expected = v;
        
        __check_result_no_return__(v.size(), expected.size());
        
        for (auto it = v.begin(), expected_it = expected.begin(); it != v.end(); ++it, ++expected_it)
            __check_result_no_return__(*it, *expected_it);
        
        return true;
    }

    void test_custom_allocator() { std::cout << "\nSize allocated: " << v.get_allocator().get_allocs() << "\n"; }

    stl::vector<T, Allocator>      v;
    const std::initializer_list<T> il;

    constexpr static stl::size_t N = 15;
};

#endif // VECTOR_TEST_H