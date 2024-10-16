#pragma once

#include "../STL/containers/forward_list/forward_list.h"

#include "custom_alloc_TT.h"
#include "UTconfig.h"

template <
    typename T,
    typename Allocator = my_alloc::custom_tracking_allocator<T>
> class forward_list_container_test
{
public:
    void __TEST__()
    {
        TEST_CASE(this->test_1());
    }

    constexpr forward_list_container_test(const std::initializer_list<T>& ilist) noexcept
        : my_forward_list(ilist) { }

    ~forward_list_container_test() = default;

private:

    bool test_1()
    {
        stl::forward_list<T, Allocator> expected(this->my_forward_list);

        __check_result_no_return__(expected.empty(), false);

        for (stl::fwd_list_iterator<T> it = this->my_forward_list.begin(), __it = expected.begin(); it != this->my_forward_list.end(), __it != expected.end(); ++it, ++__it)
            __check_result_no_return__(*it.m_node->get_m_data(), *__it.m_node->get_m_data());

        return true;
    }

    bool test_2()
    {
        
    }

    stl::forward_list<T, Allocator> my_forward_list;
    constexpr static stl::size_t N = 0;
};