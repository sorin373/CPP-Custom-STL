#ifndef __CORE_H__
#define __CORE_H__

#include "../Types/traits.h"

namespace stl
{
    template <typename ItTypeI, typename ItTypeII>
    inline bool equal(ItTypeI lhs, ItTypeI lhs_end, ItTypeII rhs)
    {
        while (lhs != lhs_end)
        {
            if (*lhs != *rhs)
                return false;

            ++lhs;
            ++rhs;
        }

        return true;
    }

    template <typename ItTypeI, typename ItTypeII, typename BinaryPredicate>
    inline bool equal(ItTypeI lhs, ItTypeI lhs_end, ItTypeII rhs, BinaryPredicate binary_pred)
    {
        while (lhs != lhs_end)
        {
            if (!binary_pred(*lhs, *rhs))
                return false;

            ++lhs;
            ++rhs;
        }

        return true;
    }

    template <typename ItTypeI, typename ItTypeII>
    inline bool equal(ItTypeI lhs, ItTypeI lhs_end, ItTypeII rhs, ItTypeII rhs_end)
    {
        while (lhs != lhs_end)
        {
            if (rhs == rhs_end)
                return false;
            
            if (*lhs != *rhs)
                return false;

            ++lhs;
            ++rhs;
        }

        return lhs == lhs_end && rhs == rhs_end;
    }

    template <typename ItTypeI, typename ItTypeII>
    inline bool lexicographical_compare(ItTypeI lhs, ItTypeI lhs_end, ItTypeII rhs, ItTypeII rhs_end)
    {
        while (lhs != lhs_end && rhs != rhs_end)
        {
            if (*lhs < *rhs)
                return true;

            if (*lhs > *rhs)
                return false;

            ++lhs;
            ++rhs;
        }

        return (lhs == lhs_end) && (rhs != rhs_end);
    }

    template <typename ItTypeI, typename ItTypeII, typename Compare>
    inline bool lexicographical_compare(ItTypeI lhs, ItTypeI lhs_end, ItTypeII rhs, ItTypeII rhs_end, Compare comp)
    {
        while (lhs != lhs_end && rhs != rhs_end)
        {
            if (comp(*lhs, *rhs))
                return true;

            if (comp(*lhs, *rhs))
                return false;

            ++lhs;
            ++rhs;
        }

        return (lhs == lhs_end) && (rhs == rhs_end);
    }

    template <typename Type>
    void swap(Type& lhs, Type& rhs)
    {
        Type aux = lhs;
        lhs = rhs;
        rhs = aux;
    }

    template <typename Type>
    void bubble_sort(Type *arr, stl::size_t size)
    {
        bool is_sorted = false;

        do
        {
            is_sorted = true;
            for (stl::size_t i = 0; i < size - 1; ++i)
                if (arr[i] > arr[i + 1])
                {
                    stl::swap(arr[i], arr[i + 1]);
                    is_sorted = false;
                }
        } while (!is_sorted);
    }
} 

#endif // CORE_H