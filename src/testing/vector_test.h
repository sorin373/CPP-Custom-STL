#ifndef __VECTOR_TEST_H__
#define __VECTOR_TEST_H__

#include "../STL/allocator.h"
#include "../STL/vector.h"

#include <iostream>

#define TEST_CASE(x) { if (!(x)) std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; else { std::cout << "Test case on line " << __LINE__ << " passed successfully!\n"; ++COUNT; } }

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
        
        test_allocator();

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

    bool test_1()
    {
        stl::vector<T, Allocator> expected = v;

        if (expected.size() != v.size())
            return false;

        for (unsigned int i = 0, size = v.size(); i < size; ++i)
            if (v[i] != expected[i])
                return false;

        return true;
    }

    void test_allocator() { std::cerr << "\nNo. of allocs: " << v.get_allocator().get_allocs() << std::endl; }

    stl::vector<T, Allocator>      v;
    const std::initializer_list<T> il;

    constexpr static unsigned int N = 3;
};

#endif // VECTOR_TEST_H