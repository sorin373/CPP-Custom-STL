#ifndef __LIST_H__
#define __LIST_H__

#include "../../allocator/allocator.h"

namespace stl
{
    template <typename T>
    struct list_node
    {
        list_node() : m_storage(), m_next(nullptr) { }

        list_node(T data) : m_storage(data), m_next(nullptr) { }

        T* get_m_data() noexcept { return &this->m_storage; }

        const T* get_m_data() const noexcept { return &this->m_storage; }

        list_node<T>* m_next;

    private:
        T m_storage;
    };

    template <typename T>
    struct list_iterator
    {

    };

    template <typename T>
    struct const_list_iterator
    {

    };

    template <
        typename T, 
        typename Allocator = stl::allocator<T>
    > class list
    {

    };
}

#include "list.tcc"

#endif // LIST_H