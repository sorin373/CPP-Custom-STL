#include <memory>

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

    template <typename T>
    void swap(T& lhs, T& rhs)
    {
        T aux = lhs;
        lhs = rhs;
        rhs = aux;
    }

    template <typename T, size_t N>
    void swap(T (&a)[N], T (&b)[N])
    {
        for (size_t i = 0; i < N; ++i)
            stl::swap(a[i], b[i]);
    }

    template <typename T>
    typename enable_if<is_trivially_copyable<T>::value, T*>::type copy(const T* first, const T* last, T* d_first)
    {
        memcpy(d_first, first, sizeof(T) * (last - first));
        return d_first + (last - first);
    }

    template <typename InputIt, typename OutputIt>
    OutputIt copy(InputIt first, InputIt last, OutputIt d_first)
    {
        for (; first != last; (void)++first, (void)++d_first)
            *d_first = *first;

        return d_first;
    }

    template <typename InputIt, typename OutputIt, typename UnaryPred>
    OutputIt copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPred pred)
    {
        for (; first != last; ++first)
            if (pred(*first))
            {
                *d_first = *first;
                ++d_first;
            }

        return d_first;
    }
}