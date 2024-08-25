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

namespace my_alloc
{
    template <typename T>
    class custom_allocator_new
    {
    public:
        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef unsigned int size_type; 

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
    };

    template <typename T>
    class custom_allocator_malloc
    {
    public:
        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef unsigned int size_type; 

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
        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef unsigned int size_type; 

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
    void __TEST__()
    {
        COUNT = 0;

        TEST_CASE(test_0());
        TEST_CASE(test_1());
        TEST_CASE(test_2());
        TEST_CASE(test_3());
        TEST_CASE(test_4());
        TEST_CASE(test_5());
        TEST_CASE(test_6());
        
        test_custom_allocator();

        std::cout << "\n" << COUNT << "/" << N << " passed!\n\n";
    }

    vector_container_test(const std::initializer_list<T>& init) : v(init), il(init) { }

private:
    bool test_0()
    {
        stl::vector<T, Allocator> expected = il;

        __check_result_no_return__(v.size(), expected.size());

        for (unsigned int i = 0, size = v.size(); i < size; ++i)
            __check_result_no_return__(v[i], expected[i]);

        return true;
    }

    bool test_1()
    {
        stl::vector<T, Allocator> expected = v;

        __check_result_no_return__(v.size(), expected.size());

        for (unsigned int i = 0, size = v.size(); i < size; ++i)
            __check_result_no_return__(v[i], expected[i]);

        return true;
    }

    bool test_2()
    {
        stl::vector<T, Allocator> expected = il;

        unsigned int index = 0;
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
        stl::vector<T, Allocator> expected = il;

        unsigned int index = 0;
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
        stl::vector<T, Allocator> expected = il;

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
        stl::vector<T, Allocator> expected = il;

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
        stl::vector<T, Allocator> expected = il;
        stl::vector<T, Allocator> user_vector;

        for (unsigned int i = 0, size = v.size(); i < size; ++i)
            user_vector.push_back(v[i]);

#if __debug_output_msg__
        __send_console_msg__(v.size());
        __send_console_msg__(user_vector.size());

        std::cout << "\n";
#endif
        __check_result_no_return__(expected.size(), user_vector.size());

        for (unsigned int i = 0, size = user_vector.size(); i < size; ++i)
            __check_result_no_return__(user_vector[i], expected[i]);

        return true;
    }

    bool test_7()
    {
        
    }

    void test_custom_allocator() { std::cout << "\nSize allocated: " << v.get_allocator().get_allocs() << "\n"; }

    stl::vector<T, Allocator>      v;
    const std::initializer_list<T> il;

    constexpr static unsigned int N = 7;
};

#endif // VECTOR_TEST_H