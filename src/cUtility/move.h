#pragma once

#include "../STL/traits/type_traits.h"

namespace stl
{
    template <typename T>
    constexpr T&& forward(typename stl::remove_reference<T>::type& t) noexcept
    {
        return static_cast<T&&>(t);
    }

    template <typename T>
    constexpr T&& forward(typename stl::remove_reference<T>::type&& t) noexcept
    {
        static_assert(!stl::is_lvalue_reference<T>::value, "template argument" " substituting T is an lvalue reference type");
        return static_cast<T&&>(t);
    }
}