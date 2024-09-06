#pragma once

#include "STL/traits/type_traits.h"

namespace cm
{
    template <typename T>
    T abs(T value)
    {
        static_assert(stl::is_arithmetic<T>::value, "The argument must be a numeric type.");

        if (value < 0)
            return -value;

        return value;
    }
}