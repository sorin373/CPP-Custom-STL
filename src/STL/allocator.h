#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include "../memory/memory.h"

#include <malloc.h>

#define ALLOCATOR_RUNTIME_ERROR throw std::runtime_error("Allocator: allocate() failed!\n"); 

namespace stl
{
    template <typename T> 
    class allocator
    {
        typedef T                 value_type;
        typedef size_t            size_type;
        typedef value_type*       pointer;
        typedef const value_type* const_pointer;

    public:
        allocator() noexcept { }

        constexpr void *allocate(size_type n = 1)
        {
            return malloc(sizeof(value_type) * n);
        }

        void n_deallocate(void *p)
        {
            operator delete(p);
        }

        void deallocate(void *p)
        {
            free(p);
        }

        ~allocator() { }
    };
}

#endif