#ifndef __HASHABLE_H__
#define __HASHABLE_H__

#include "stl_pair.h"

namespace stl
{
    namespace __detail
    {
        template <typename Key, typename T> 
        struct __hash_node
        {
            pair<Key, T>  m_pair;
            __hash_node*        m_next;

            __hash_node() noexcept
                : m_pair(), m_next(nullptr) { }

            __hash_node(const Key& key, const T& value)
                : m_pair(key, value), m_next(nullptr) { }

            __hash_node(const __hash_node& other)
                : m_pair(other.m_pair), m_next(other.m_next) { }

            Key first() const noexcept { return this->m_pair.first; }

            T second() const noexcept { return this->m_pair.second; }
        };

        template <typename Key, typename T>
        struct __node_iterator
        {   
            typedef __hash_node<Key, T>  value_type;
            typedef value_type*          pointer;
            typedef value_type&          reference;

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
            typedef const __hash_node<Key, T>  value_type;
            typedef value_type*                pointer;
            typedef value_type&                reference;

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
            typedef __hash_node<Key, T>  value_type;
            typedef value_type*          pointer;
            typedef value_type&          reference;

            pointer*  m_bucket_begin;
            pointer*  m_bucket_end;
            pointer   m_current;

            __map_iterator() noexcept
                : m_bucket_begin(nullptr), m_bucket_end(nullptr), m_current(nullptr) { }

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
            typedef const __hash_node<Key, T>  value_type;
            typedef value_type*                pointer;
            typedef value_type&                reference;

            pointer*  m_bucket_begin;
            pointer*  m_bucket_end;
            pointer   m_current;

            __const_map_iterator() noexcept
                : m_bucket_begin(nullptr), m_bucket_end(nullptr), m_current(nullptr) { }

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

    /**
     * @brief Allias describing a map node which wraps around a @c stl::pair type.
     * @tparam Key key type @tparam T mapped type
     */
    template <typename Key, typename T> 
    using pair_node = __detail::__hash_node<Key, T>;

    /**
     * @brief Allias describing a node iterator which wraps around a @c stl::pair_node type.
     * @tparam Key key type @tparam T mapped type
     */
    template <typename Key, typename T>
    using node_iterator = __detail::__node_iterator<Key, T>;

    /**
     * @brief Allias describing a constant node iterator which wraps around a @c stl::pair_node type.
     * @tparam Key key type @tparam T mapped type
     */
    template <typename Key, typename T>
    using const_node_iterator = __detail::__const_node_iterator<Key, T>;

    /**
     * @brief Allias describing a map iterator which wraps around a @c stl::pair_node type.
     *       This iterator goes accross buckets.
     * @tparam Key key type @tparam T mapped type
     */
    template <typename Key, typename T>
    using map_iterator = __detail::__map_iterator<Key, T>;

    /**
     * @brief Allias describing a constant map iterator which wraps around a @c stl::pair_node type.
     *       This iterator goes accross buckets.
     * @tparam Key key type @tparam T mapped type
     */
    template <typename Key, typename T>
    using const_map_iterator = __detail::__const_map_iterator<Key, T>;
}

#endif // HASHABLE_H