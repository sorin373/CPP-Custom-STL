#pragma once

#include "../STL/traits/type_traits.h"

namespace stl
{
    template <typename T>
    constexpr T&& forward(typename stl::remove_reference<T>::type& t) noexcept
    { return static_cast<T&&>(t); }

    template <typename T>
    constexpr T&& forward(typename stl::remove_reference<T>::type&& t) noexcept
    {
        static_assert(!stl::is_lvalue_reference<T>::value, "template argument" " substituting T is an lvalue reference type");
        return static_cast<T&&>(t);
    }

    template <typename T>
    inline T* addressof(T& __r) noexcept 
    { return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(__r))); }

    template <typename T>
    constexpr typename stl::remove_reference<T>::type&& move(T&& __t) noexcept 
    { return static_cast<typename stl::remove_reference<T>::type&&>(__t); }

    template <typename T, typename Up = T>
    inline T exchange(T& __obj, Up&& __new_val)
    {
        T __old_val = stl::move(__obj);
        __obj = stl::forward<Up>(__new_val);
        return __old_val;
    }
}