#ifndef __UNORDERED_MAP_TEST_H__
#define __UNORDERED_MAP_TEST_H__

#include "../STL/containers/unordered_map/unordered_map.h"

#include "custom_alloc_TT.h"
#include "UTconfig.h"

template <
    typename Key,
    typename T,
    typename Hash = stl::hash<Key>,
    typename KeyEqual = stl::equal_to<Key>,
    typename Allocator = stl::allocator<stl::pair_node<Key, T>>
> class unordered_map_container_test
{
public:
    void __TEST__()
    {
        TEST_CASE(this->test_1());
    }

    constexpr unordered_map_container_test(const std::initializer_list<T>& ilist)
        : my_u_map(ilist) { }

    ~forward_list_container_test() = default;

private:

    bool test_1()
    {

    }

    stl::unordered_map<Key, T, Hash, KeyEqual, Allocator> my_u_map;
    constexpr static stl::size_t N = 0;
};

#endif // UNORDERED_MAP_TEST_H