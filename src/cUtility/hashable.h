#ifndef __HASHABLE_H__
#define __HASHABLE_H__

#include "stl_pair.h"

namespace stl
{
    template <typename Key, typename T> 
    struct __hash_node
    {
        pair<const Key, T>  m_pair;
        __hash_node*        m_next;

        __hash_node() noexcept
            : m_pair(), m_next(nullptr) { }

        __hash_node(const Key& key, const T& value)
            : m_pair(key, value), m_next(nullptr) { }

        __hash_node(const __hash_node& other)
            : m_pair(other.m_pair), m_next(other.m_next) { }
    };

    template <typename Key, typename T>
    struct __node_iterator
    {   
        typedef __hash_node<const Key, T>  value_type;
        typedef value_type*                pointer;
        typedef value_type&                reference;

        pointer m_current;

        __node_iterator(pointer __x) 
            : m_current(__x) { }

        reference operator*() { return *this->m_current; }

        pointer operator->() { return this->m_current; }

        __node_iterator& operator++() noexcept
        {
            if (m_current != nullptr)
                m_current = m_current->m_next;

            return *this;
        }

        __node_iterator& operator++(int) noexcept
        {
            __node_iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const __node_iterator& other) const noexcept { return this->m_current == other.m_current; }
        bool operator!=(const __node_iterator& other) const noexcept { return this->m_current != other.m_current; }
    };

    template <typename Key, typename T>
    struct __const_node_iterator
    {
        typedef const __hash_node<const Key, T>   value_type;
        typedef value_type*                       pointer;
        typedef value_type&                       reference;

        pointer m_current;

        __const_node_iterator(pointer __x) 
            : m_current(__x) { }

        reference operator*() const { return *this->m_current; }

        pointer operator->() const { return this->m_current; }

        __const_node_iterator& operator++() noexcept
        {
            if (m_current != nullptr)
                m_current = m_current->m_next;

            return *this;
        }

        __const_node_iterator& operator++(int) noexcept
        {
            __const_node_iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const __const_node_iterator& other) const { return this->m_current == other.m_current; }
        bool operator!=(const __const_node_iterator& other) const { return this->m_current != other.m_current; }
    };

    template <typename Key, typename T>
    struct __map_iterator
    {
        typedef __hash_node<const Key, T>  value_type;
        typedef value_type*                pointer;
        typedef value_type&                reference;

        pointer*  m_bucket_begin;
        pointer*  m_bucket_end;
        pointer   m_current;

        __map_iterator(pointer* __bucket_begin, pointer* __bucket_end, pointer __node_it) 
            : m_bucket_begin(__bucket_begin), m_bucket_end(__bucket_end), m_current(__node_it) 
        { 
            if (this->m_current == nullptr && this->m_bucket_begin != this->m_bucket_end)
                ++(*this);
        }

        reference operator*() const { return *this->m_current; }

        pointer operator->() const { return this->m_current; }

        __map_iterator& operator++()
        {
            if (this->m_current == nullptr)
                this->m_current = this->m_current->m_next;
            else
            {
                while (this->m_current == nullptr && this->m_bucket_begin != this->m_bucket_end)
                {
                    ++this->m_bucket_begin;

                    if (this->m_bucket_begin != this->m_bucket_end)
                        this->m_current = *this->m_bucket_begin;
                }
            }

            return *this;
        }

        __map_iterator operator++(int)
        {
            __map_iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const __map_iterator& other) 
        { return this->m_bucket_begin = other.m_bucket_begin && this->m_bucket_end == other.m_bucket_end && this->m_current == other.m_current; }

        bool operator!=(const __map_iterator& other)
        { return !(*this == other); }
        
    };

    template <typename Key, typename T>
    struct __const_map_iterator
    {
        typedef const __hash_node<const Key, T>  value_type;
        typedef value_type*                      pointer;
        typedef value_type&                      reference;

        pointer*  m_bucket_begin;
        pointer*  m_bucket_end;
        pointer   m_current;

        __const_map_iterator(pointer* __bucket_begin, pointer* __bucket_end, pointer __node_it)
            : m_bucket_begin(__bucket_begin), m_bucket_end(__bucket_end), m_current(__node_it) 
        {
            if (this->m_current == nullptr && this->m_bucket_begin != this->m_bucket_end)
                ++(*this);
        }

        reference operator*() const { return *this->m_current; }

        pointer operator->() const { return this->m_current; }

        __const_map_iterator& operator++()
        {
            if (this->m_current != nullptr)
                this->m_current = this->m_current->m_next;
            else
            {
                while (this->m_current == nullptr && this->m_bucket_begin != this->m_bucket_end)
                {
                    ++this->m_bucket_begin;

                    if (this->m_bucket_begin != this->m_bucket_end)
                        this->m_current = *this->m_bucket_begin;
                }
            }

            return *this;
        }

        __const_map_iterator& operator++(int)
        {
            __const_map_iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const __const_map_iterator& other) 
        { return this->m_bucket_begin = other.m_bucket_begin && this->m_bucket_end == other.m_bucket_end && this->m_current == other.m_current; }

        bool operator!=(const __const_map_iterator& other)
        { return !(*this == other); }
    };
}

#endif // HASHABLE_H