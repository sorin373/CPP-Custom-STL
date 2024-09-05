#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include "../algorithm/algorithm.h"

namespace stl
{
    template <typename T>
    class allocator
    {
    public:
        typedef T           value_type;
        typedef stl::size_t size_type;

        typedef T*          pointer;
        typedef const T*    const_pointer;

        typedef T&          reference;
        typedef const T&    const_reference;
        
        template <typename U>
        struct rebind
        {
            typedef allocator<U> other;
        };

        pointer address(reference value) const { return &value; }

        const_pointer address(const_reference value) const { return &value; }

        allocator() noexcept { }

        allocator(const allocator& other) noexcept { }

        template <typename U>
        allocator(const allocator<U>& other) noexcept { }

        ~allocator() noexcept { }

        size_type max_size() const noexcept;

        pointer allocate(size_type size, const void* hint = nullptr);

        void deallocate(pointer ptr, size_type size);

        void construct(pointer ptr, const_reference value);

        void destroy(pointer ptr) { ptr->~T(); }
    };

    template <typename TypeI, typename TypeII>
    bool operator==(const allocator<TypeI>&, const allocator<TypeII>&) throw() { return true; }

    template <typename TypeI, typename TypeII>
    bool operator!=(const allocator<TypeI>&, const allocator<TypeII>&) throw() { return false; }
}

#include "allocator.tcc"

#endif // ALLOCATOR_H