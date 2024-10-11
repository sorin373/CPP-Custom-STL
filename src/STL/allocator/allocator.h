#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include "../../cUtility/move.h"
#include "../traits/type_traits.h"
#include "../traits/allocator_traits.h"

namespace stl
{
    template <typename T>
    struct allocator
    {
        typedef T               value_type;
        typedef T*              pointer;
        typedef const T*        const_pointer;
        typedef T&              reference;
        typedef const T&        const_reference;
        typedef stl::size_t     size_type;
        typedef stl::ptrdiff_t  difference_type;
        typedef true_type       propagate_on_container_move_assignment;
        typedef void*           void_pointer;
        typedef const void*     const_void_pointer;
        
        template <typename U> 
        struct rebind { typedef allocator<U> other; };

        typedef true_type is_always_equal;

        pointer address(reference value) const { return &value; }

        const_pointer address(const_reference value) const { return &value; }

        allocator() noexcept { }

        allocator(const allocator& other) noexcept { }

        template <typename U>
        allocator(const allocator<U>& other) noexcept { }

        ~allocator() noexcept { }

        size_type max_size() const noexcept;

        pointer allocate(size_type size, const_void_pointer hint = nullptr);

        void deallocate(pointer ptr, size_type size);

        void construct(pointer ptr, const_reference value);

        template <typename... Args>
        void construct(pointer ptr, Args&&... args);

        void destroy(pointer ptr) { ptr->~T(); }
    };

    template <typename TypeI, typename TypeII>
    bool operator==(const allocator<TypeI>&, const allocator<TypeII>&) throw() { return true; }

    template <typename TypeI, typename TypeII>
    bool operator!=(const allocator<TypeI>&, const allocator<TypeII>&) throw() { return false; }
}

#include "allocator.tcc"

#endif // ALLOCATOR_H