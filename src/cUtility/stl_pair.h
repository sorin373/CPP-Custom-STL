#ifndef __STL_PAIR_H__
#define __STL_PAIR_H__

#include "../STL/algorithm/algorithm.h"

namespace stl
{
    template <typename T1, typename T2>
    struct pair
    {
        typedef T1 first_type;
        typedef T2 second_type;

        first_type first;
        second_type second;

        constexpr pair() 
            : first(), second() { }

        constexpr pair(const first_type& x, const second_type& y)
            : first(x), second(y) { }

        constexpr pair(const pair& other)
            : first(other.first), second(other.second) { }

        constexpr pair& operator=(const pair& other)
        {
            this->first = other.first;
            this->second = other.second;
        }

        void swap(pair& p)
        {
            stl::swap(this->first, p.first);
            stl::swap(this->second, p.second);
        }
    };

    template <typename T1, typename T2>
    inline constexpr bool operator==(const pair<T1, T2>& lhs, const pair<T1, T2>& rhs)
    { return lhs.first == rhs.first && lhs.second == rhs.second; }

    template <typename T1, typename T2>
    inline void swap(pair<T1, T2>& lhs, pair<T1, T2>& rhs) noexcept(noexcept(lhs.swap(rhs)))
    { lhs.swap(rhs); }
}

#endif // STL_PAIR_H