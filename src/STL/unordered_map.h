/**
 * @copyright MIT License

    Copyright (c) 2024 Sorin Tudose

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

    @file unordered_map.h

    @brief This is part of the standard template libraries (STL) and it is used to implement an unordered map.
 */

#ifndef __UNORDERED_MAP__
#define __UNORDERED_MAP__

#include "reverse_iterator.h"

#include <malloc.h>
#include <stdexcept>
#include <initializer_list>

#define MALLOC_RUNTIME_ERROR throw std::runtime_error("malloc: couldn't allocate memory!\n");

/**
 * @todo
 * -> emplace()
 * -> find()
 * -> count()
 * -> erase()
 * -> clear()
 * -> operator[]
 * -> swap()
 */

namespace stl
{
    template <typename K, typename V> class pair
    {
        typedef V     value_type;
        typedef K     key_type;
        typedef pair* pointer;

        typedef V& reference;

    public:
        pair(const key_type key, const value_type value)
            : m_key(key), m_value(value), m_next(nullptr) { }

        key_type get_key() const noexcept         { return m_key; }

        value_type get_value() const noexcept     { return m_value; }

        reference get_value() noexcept            { return m_value; }

        pointer get_next() const noexcept         { return m_next; }

        void set_value(value_type value) noexcept { m_value = value; }

        void set_next(pointer next) noexcept      { m_next = next; }

    private:
        key_type     m_key;
        value_type   m_value;
        pointer      m_next;
    };

    /**
     * @brief Default hashing function. The user can override this but the function header needs to be kept.
     * @param K          Key type
     * @param key        key
     * @param table_size The size of the hash table
     * @returns The hash value of an element.
     */
    template <typename K> class key_hash
    {
    public:
        size_t operator()(const K &key, const unsigned int table_size) const noexcept
        {
            return (size_t)(key) % table_size;
        }
    };

    template <typename K> class key_equal
    {
    public:
        bool operator()(const K &lhs, const K &rhs)
        {
            return lhs == rhs;
        }
    };

    /**
     * @brief This container represents a generic implementation of the hash table data structure.
     *        It consists in many member functions that help the user manage the hash table data faster and more efficiently.
     *        Moreover, it has automatic resizing when the load factor exceeds 75%.
     * @param K Key type
     * @param V Value type
     * @param _hash Hash function
     * @param _key_equal Provides a function for key comparison
     */
    template <typename K, typename V, typename _hash = key_hash<K>, typename _key_equal = key_equal<K>> class unordered_map
    {
        typedef _hash      hf_type;
        typedef _key_equal ke_type;
        typedef V          value_type;
        typedef K          key_type;

        typedef unsigned int size_type;

        typedef       value_type&       value_reference;
        typedef const value_type& const_value_reference;

        typedef       pair<key_type, value_type>*       pointer;
        typedef const pair<key_type, value_type>* const_pointer;

        constexpr static size_type INITIAL_CAPACITY = 16U;
        constexpr static double LOAD_FACTOR = 0.75;

    public:
        typedef       pair<key_type, value_type>**       bucket_iterator;
        typedef const pair<key_type, value_type>** const_bucket_iterator;

        typedef stl::reverse_iterator<pair<key_type, value_type>*>             bucket_reverse_iterator;
        typedef stl::const_reverse_iterator<pair<key_type, value_type>*> const_bucket_reverse_iterator;

        unordered_map(size_type bucket_count = INITIAL_CAPACITY) 
            : m_table(nullptr), m_size(0), m_capacity(bucket_count)
        { 
            m_table = (pair<key_type, value_type>**)malloc(m_capacity * sizeof(pair<key_type, value_type>*));

            if (m_table == nullptr) MALLOC_RUNTIME_ERROR

            for (size_type i = 0; i < m_capacity; i++)
                m_table[i] = 0;
        }

        unordered_map(const unordered_map &other) 
            : m_table(nullptr), m_size(0), m_capacity(other.bucket_count())
        {
            m_table = (pair<key_type, value_type>**)malloc(m_capacity * sizeof(pair<key_type, value_type>*));

            if (m_table == nullptr) MALLOC_RUNTIME_ERROR

            for (size_type i = 0; i < m_capacity; i++)
                m_table[i] = 0;

            for (size_type i = 0, n = other.bucket_count(); i < n; i++)
            {
                pointer entry = m_table[i];

                while (entry != nullptr)
                {
                    insert(entry->get_key(), entry->get_value());
                    entry = entry->get_next();
                }
            }
        }

        unordered_map(std::initializer_list<pair<key_type, value_type>> init, size_type bucket_count = INITIAL_CAPACITY) 
            : m_table(nullptr), m_size(0), m_capacity(bucket_count)
        {
            m_table = (pair<key_type, value_type>**)malloc(m_capacity * sizeof(pair<key_type, value_type>*));

            if (m_table == nullptr) MALLOC_RUNTIME_ERROR

            for (size_type i = 0; i < m_capacity; i++)
                m_table[i] = 0;

            for (pair<key_type, value_type> it : init)
                insert(it.get_key(), it.get_value());
        }

        unordered_map& operator=(const unordered_map other)
        {
            if (this != &other)
            {
                m_capacity = other.bucket_count();
                m_size = other.size();

                for (size_type i = 0; i < m_capacity; i++)
                {
                    pointer entry = other.get_m_table()[i];

                    while (entry != nullptr)
                    {
                        insert(entry->get_key(), entry->get_value());
                        entry = entry->get_next();
                    }
                }   
            }
                     
            return *this;
        }

        unordered_map& operator=(std::initializer_list<pair<key_type, value_type>> init)
        {
            m_capacity = INITIAL_CAPACITY;
            m_size = 0;

            m_table = (pair<key_type, value_type>**)malloc(m_capacity * sizeof(pair<key_type, value_type>*));

            if (m_table == nullptr) MALLOC_RUNTIME_ERROR

            for (size_type i = 0; i < m_capacity; i++)
                m_table[i] = 0;

            for (pair<key_type, value_type> it : init)
                insert(it.get_key(), it.get_value());

            return *this;
        }

        void resize(size_type new_size)
        {
            pair<key_type, value_type> **temp_table = (pair<key_type, value_type>**)malloc(new_size * sizeof(pair<key_type, value_type>*));

            if (temp_table == nullptr) MALLOC_RUNTIME_ERROR

            for (size_type i = 0; i < new_size; i++)
                m_table[i] = 0;

            for (size_type i = 0; i < m_size; i++)
            {
                pointer entry = m_table[i];

                while (entry != nullptr)
                {
                    pointer next = entry->get_next();
                    size_t new_hash_value = m_hash_func(entry->get_key(), new_size);

                    entry->set_next(temp_table[new_hash_value]);

                    temp_table[new_hash_value] = entry;

                    entry = next;
                }
            }

            free(m_table);
            m_table = temp_table;
        }

        bool find(const key_type key, value_reference value)
        {
            pointer entry = m_table[m_hash_func(key, m_capacity)];

            while (entry != nullptr)
            {
                if (m_key_equal(entry->get_key(), key))
                {
                    value = entry->get_value();
                    return true;
                }

                entry = entry->get_next();
            }

            return false;
        }

        void clear() noexcept
        {
            for (size_type i = 0; i < m_capacity; ++i)
            {
                pointer entry = m_table[i];

                while (entry != nullptr)
                {
                    pointer temp = entry;
                    entry = entry->get_next();

                    delete temp;
                }

                m_table[i] = nullptr; 
            }

            m_size = 0;
        }

        void insert(const key_type key, const value_type value)
        {
            // check the load factor to see if the hash table needs resizing. (to do: not sure this is the right approach) 
            // LOAD_FACTOR = 75% (predefined)
            if ((double)(m_size) / m_capacity > LOAD_FACTOR)
            {
                m_capacity *= 2;
                resize(m_capacity);
            }

            size_type hash_value = m_hash_func(key, m_capacity);

            pointer prev  = nullptr;             // Keeps track of the last found pair in the forward list when the while is terminated
            pointer entry = m_table[hash_value]; // select the entry point for the forward list

            // transverse the forward list to the end while looking for the key
            while (entry != nullptr && !m_key_equal(entry->get_key(), key))
            {
                prev = entry;
                entry = entry->get_next();
            }
            
            // if the entry is null that means the end of the list was reached and the key was not found
            if (entry == nullptr)
            {
                // create a new pair for the new element
                entry = new pair<key_type, value_type>(key, value);
                
                // if prev is null that means the entry was null from the begining and there
                // is not a list currently at that specific hash value. Therefore that bucket is assigned the address of the entry
                if (prev == nullptr)
                    m_table[hash_value] = entry;
                else
                    // if prev != null that means that there is a list at the hash value and the entry is added at the back of this list
                    prev->set_next(entry);

                // m_size gets incremented for each entry
                m_size++;
            }
            else
                // if entry is not null that means the prev while was terminated when the same key was found.
                // Therefore the value is just updated for this specific key
                entry->set_value(value);
        }

        bool empty() const noexcept
        {
            for (size_type i = 0; i < m_capacity; i++)
                if (m_table[i] != nullptr)
                    return false;

            return true;
        }

        pair<key_type, value_type>** get_m_table() const noexcept { return m_table; }

        size_type size() const noexcept         { return m_size; }

        size_type bucket_count() const noexcept { return m_capacity; }

        size_type bucket(const key_type key)
        {   
            size_type hash_value = m_hash_func(key, m_capacity);
            pointer entry = m_table[hash_value];

            while (entry != nullptr)
            {
                if (m_key_equal(entry->get_key(), key))
                    return hash_value;

                entry = entry->get_next();
            }
        }

        value_reference at(const key_type key)
        {
            pointer entry = m_table[m_hash_func(key, m_capacity)];

            while (entry != nullptr)
            {   
                if (m_key_equal(entry->get_key(), key))
                    return entry->get_value();

                entry = entry->get_next();
            }

            throw std::out_of_range("Key not found!\n");
        }
        
        class iterator
        {
        public:
            iterator(bucket_iterator bucket_begin, bucket_iterator bucket_end, pointer node_it)
                : m_bucket_begin(bucket_begin), m_bucket_end(bucket_end), m_node_it(node_it) 
                { 
                    // if node_it was null, the first bucket which is not null is searched using ++ operator defined below
                    if (m_node_it == nullptr && m_bucket_begin != m_bucket_end)
                        ++(*this);
                }
            
            pair<key_type, value_type>& operator*() const  { return *m_node_it; }

            pair<key_type, value_type>* operator->() const { return m_node_it; }

            iterator& operator++() // prefix
            {
                if (m_node_it != nullptr)
                    m_node_it = m_node_it->get_next();

                while (m_node_it == nullptr && m_bucket_begin != m_bucket_end)
                {
                    m_bucket_begin++;
                    
                    if (m_bucket_begin != m_bucket_end)
                        m_node_it = *m_bucket_begin;
                }

                return *this;
            }

            iterator operator++(int) // postfix
            {
                iterator temp = *this;

                ++(*this);

                return temp;
            }

            bool operator==(const iterator &other) { return m_bucket_begin == other.m_bucket_begin && m_node_it == other.m_node_it; }

            bool operator!=(const iterator &other) { return !(*this == other); }

        private:
            bucket_iterator m_bucket_begin;
            bucket_iterator m_bucket_end;
            pointer         m_node_it;
        };

        class const_iterator
        {
        public:
            const_iterator(const_bucket_iterator bucket_begin, const_bucket_iterator bucket_end, const_pointer node_it)
                : m_bucket_begin(bucket_begin), m_bucket_end(bucket_end), m_node_it(node_it) 
                { 
                    if (m_node_it == nullptr && m_bucket_begin != m_bucket_end)
                        ++(*this);
                }

                const pair<key_type, value_type>& operator*() const  { return *m_node_it; }

                const pair<key_type, value_type>* operator->() const { return m_node_it; }

                const_iterator& operator++()
                {
                    if (m_node_it != nullptr)
                        m_node_it = m_node_it->get_next();

                    while (m_node_it == nullptr && m_bucket_begin != m_bucket_end)
                    {
                        m_bucket_begin++;
                        
                        if (m_bucket_begin != m_bucket_end)
                            m_node_it = *m_bucket_begin;
                    }

                    return *this;
                }

                const_iterator operator++(int)
                {
                    const_iterator temp = *this;

                    ++(*this);

                    return temp;
                }

                bool operator==(const iterator &other) { return m_bucket_begin == other.m_bucket_begin && m_node_it == other.m_node_it; }

                bool operator!=(const iterator &other) { return !(*this == other); }

        private:
            const_bucket_iterator m_bucket_begin;
            const_bucket_iterator m_bucket_end;
            const_pointer         m_node_it;
        };

        iterator begin() { return iterator(m_table, m_table + m_capacity, m_table[0]); }

        iterator end()   { return iterator(m_table + m_capacity, m_table + m_capacity, nullptr); }

        const_iterator cbegin() const noexcept { return const_iterator(m_table, m_table + m_capacity, m_table[0]); }

        const_iterator cend() const noexcept   { return const_iterator(m_table + m_capacity, m_table + m_capacity, nullptr); }

        bucket_reverse_iterator rbegin() { return bucket_reverse_iterator(m_table + m_capacity); }

        bucket_reverse_iterator rend()   { return bucket_reverse_iterator(m_table); }

        const_bucket_reverse_iterator crbegin() const noexcept { return const_bucket_reverse_iterator(m_table + m_capacity); }

        const_bucket_reverse_iterator crend() const noexcept   { return const_bucket_reverse_iterator(m_table); }

        ~unordered_map() 
        {
            for (size_type i = 0; i < m_size; i++)
            {
                pointer entry = m_table[i];

                while (entry != nullptr)
                {
                    pointer temp = entry;
                    entry = entry->get_next();

                    delete temp; // the nodes are allocated using the new operator
                }
            }
            
            free(m_table);
        }

    private:
        pair<key_type, value_type>** m_table;
        size_type                    m_size;     // total number of elements. It is incremented everytime a new <key, value> element is added
        size_type                    m_capacity; // total number of buckets
        hf_type                      m_hash_func;
        ke_type                      m_key_equal;
    };
}

#endif // HASH_MAP_H