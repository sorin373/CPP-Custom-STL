#ifndef __CORE_H__
#define __CORE_H__

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
}

#endif