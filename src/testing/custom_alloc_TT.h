#pragma once

#include "../STL/traits/type_traits.h"

namespace my_alloc
{
    template <typename T>
    class custom_allocator_new
    {
    public:
        typedef T                   value_type;
        typedef T*                  pointer;
        typedef const T*            const_pointer;
        typedef T&                  reference;
        typedef const T&            const_reference;
        typedef stl::size_t         size_type;
        typedef stl::ptrdiff_t      difference_type;
        typedef stl::true_type      propagate_on_container_move_assignment;
        typedef stl::true_type      is_always_equal;
        typedef void*               void_pointer;
        typedef const void*         const_void_pointer;
        
        template <typename U> 
        struct rebind { typedef custom_allocator_new<U> other; };

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
        typedef T                   value_type;
        typedef T*                  pointer;
        typedef const T*            const_pointer;
        typedef T&                  reference;
        typedef const T&            const_reference;
        typedef stl::size_t         size_type;
        typedef stl::ptrdiff_t      difference_type;
        typedef stl::true_type      propagate_on_container_move_assignment;
        typedef stl::true_type      is_always_equal;
        typedef void*               void_pointer;
        typedef const void*         const_void_pointer;

        template <typename U> 
        struct rebind { typedef custom_allocator_malloc<U> other; };

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
        typedef T                   value_type;
        typedef T*                  pointer;
        typedef const T*            const_pointer;
        typedef T&                  reference;
        typedef const T&            const_reference;
        typedef stl::size_t         size_type;
        typedef stl::ptrdiff_t      difference_type;
        typedef stl::true_type      propagate_on_container_move_assignment;
        typedef stl::true_type      is_always_equal;
        typedef void*               void_pointer;
        typedef const void*         const_void_pointer;

        template <typename U> 
        struct rebind { typedef custom_tracking_allocator<U> other; };

        constexpr custom_tracking_allocator() noexcept : m_allocs(0) { }

        custom_tracking_allocator(const custom_tracking_allocator& other) noexcept { }

        template <typename U>
        custom_tracking_allocator(const custom_tracking_allocator<U>& other) noexcept { }

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

        template <typename... Args>
        void construct(pointer ptr, Args&&... args)
        { ::new(static_cast<void*>(ptr)) T(stl::forward<Args>(args)...); }

        void destroy(pointer ptr) { ptr->~T(); }

    private:
        size_type m_allocs;
    };
}