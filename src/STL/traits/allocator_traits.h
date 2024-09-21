#pragma once

#include "../allocator/allocator.h"

namespace stl
{
    template <typename Alloc>
    struct allocator_traits
    {
        typedef Alloc                                                    allocator_type;
        typedef typename Alloc::value_type                               value_type;
        typedef typename Alloc::pointer                                  pointer;
        typedef typename Alloc::const_pointer                            const_pointer;
        typedef typename Alloc::void_pointer                             void_pointer;
        typedef typename Alloc::const_void_pointer                       const_void_pointer;
        typedef typename Alloc::difference_type                          difference_type;
        typedef typename Alloc::size_type                                size_type;
        // typedef typename Alloc::propagate_on_container_copy_assignment   propagate_on_container_copy_assignment;
        typedef typename Alloc::propagate_on_container_move_assignment   propagate_on_container_move_assignment;
        // typedef typename Alloc::propagate_on_container_swap              propagate_on_container_swap;
        typedef typename Alloc::is_always_equal                          is_always_equal;

    private:
        template <typename Alloc2>
        static auto _S_allocate(Alloc2& __a, size_type __n, const_void_pointer __hint, int) 
        -> decltype(__a.allocate(__n, __hint)) { return __a.allocate(); }

        template <typename Alloc2>
        static pointer _S_allocate(Alloc2& __a, size_type __n, const_void_pointer, ...) { return __a.allocate(__n); }

        template <typename T, typename... Args>
        struct construct_helper
        {   
            // Checks if allocator's construct method exists
            template <typename Alloc2, typename = decltype(stl::declval<Alloc2*>()->construct(stl::declval<T*>(), stl::declval<Args>()...))>
            static true_type __test(int);  // Returns `true_type` if construct exists.

            template <typename>
            static false_type __test(...); // Returns `false_type` if construct doesn't exist.

            // `type` will be `true_type` or `false_type` based on which test matches
            using type = decltype(__test<Alloc>(0));
        };

        template <typename T, typename... Args>
        using has_construct = typename construct_helper<T, Args...>::type;

        // SFINAE Constraint: the allocator has a construct method that can be used with T and Args...
        template <typename T, typename... Args>
        static require<has_construct<T, Args...>> _S_construct(Alloc& __a, T* __p, Args&&... __args) { return __a.construct(__p, stl::forward<Args>(__args)...); }

        template <typename Alloc2>
        static auto _S_select(Alloc2& __a, int)
        -> decltype(__a.select_on_container_copy_construction()) { return __a.select_on_container_copy_construction(); }

        template <typename Alloc2>
        static Alloc2 _S_select(Alloc2& __a, ...) { return __a; }

        template <typename Alloc2, typename T>
        static auto _S_destroy(Alloc2& __a, T* __p, int)
        -> decltype(__a.destroy(__p)) { __a.destroy(__p); }

        template <typename Alloc2, typename T>
        static void _S_destroy(Alloc2&, T* __p, ...) { __p->~T(); }

    public:
        static pointer allocate(Alloc& __a, size_type __n) { return __a.allocate(__n); }

        static pointer allocate(Alloc& __a, size_type __n, const_void_pointer __hint) { return _S_allocate(__a, __n, __hint, 0); }

        static void deallocate(Alloc& __a, pointer __p, size_type __n) { __a.deallocate(__p, __n); }

        template <typename T, typename... Args>
        static auto construct(Alloc& __a, T* __p, Args&&... __args) 
        -> decltype(this->_S_construct(__a, __p, stl::forward<Args>(__args)...)) { _S_construct(__a, __p, stl::forward<Args>(__args)...); }

        template <typename T>
        static void destroy(Alloc& __a, T* __p) { _S_destroy(__a, __p, 0); }
        
        static Alloc select_on_container_copy_construction(const Alloc& __rhs) { return _S_select(__rhs, 0); }
    };
}