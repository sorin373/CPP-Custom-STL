#pragma once

#include "../traits/type_traits.h"

namespace stl
{
    template <typename forwardIt, typename T>
    bool binary_search(forwardIt first, forwardIt last, const T& value);

    template <typename ItTypeI, typename ItTypeII> 
    inline bool equal(ItTypeI lhs, ItTypeI lhs_end, ItTypeII rhs);

    template <typename ItTypeI, typename ItTypeII, typename BinaryPredicate>
    inline bool equal(ItTypeI lhs, ItTypeI lhs_end, ItTypeII rhs, BinaryPredicate binary_pred);

    template <typename ItTypeI, typename ItTypeII>
    inline bool equal(ItTypeI lhs, ItTypeI lhs_end, ItTypeII rhs, ItTypeII rhs_end);

    template <typename ItTypeI, typename ItTypeII>
    inline bool lexicographical_compare(ItTypeI lhs, ItTypeI lhs_end, ItTypeII rhs, ItTypeII rhs_end);

    template <typename ItTypeI, typename ItTypeII, typename Compare>
    inline bool lexicographical_compare(ItTypeI lhs, ItTypeI lhs_end, ItTypeII rhs, ItTypeII rhs_end, Compare comp);

    template <typename T>
    void swap(T& lhs, T& rhs);

    template <typename T, size_t N>
    void swap(T (&a)[N], T (&b)[N]);

    template <typename T>
    typename enable_if<is_trivially_copyable<T>::value, T*>::type copy(const T* first, const T* last, T* d_first);

    template <typename InputIt, typename OutputIt>
    OutputIt copy(InputIt first, InputIt last, OutputIt d_first);

    template <typename InputIt, typename OutputIt, typename UnaryPred>
    OutputIt copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPred pred);
}

#include "algorithm.tcc"