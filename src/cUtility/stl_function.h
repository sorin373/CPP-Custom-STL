#pragma once

namespace stl
{
    template <typename T>
    struct equal_to
    { constexpr bool operator()(const T& lhs, const T& rhs) { return lhs == rhs; } };

    template <typename T>
    struct not_equal_to
    { constexpr bool operator()(const T& lhs, const T& rhs) { return lhs != rhs; } };

    template <typename T>
    struct greater
    { constexpr bool operator()(const T& lhs, const T& rhs) { return lhs > rhs; } };

    template <typename T>
    struct less
    { constexpr bool operator()(const T& lhs, const T& rhs) { return lhs < rhs; } };

    template <typename T>
    struct greater_equal
    { constexpr bool operator()(const T& lhs, const T& rhs) { return lhs >= rhs; } };

    template <typename T>
    struct less_equal
    { constexpr bool operator()(const T& lhs, const T& rhs) { return lhs <= rhs; } };
}