#ifndef __HASH_TABLE__
#define __HASH_TABLE__

#include "reverse_iterator.h"

#include <malloc.h>
#include <stdexcept>

#define MALLOC_RUNTIME_ERROR throw std::runtime_error("malloc: couldn't allocate memory!\n");

// to do 
// -- reverse iterators

namespace stl
{
    template <typename K, typename V> class node
    {
        typedef V            value_type;
        typedef K            key_type;

    public:
        node(const key_type key, const value_type value)
            : m_key(key), m_value(value), m_next(nullptr) { }

        key_type get_key() const noexcept         { return m_key; }

        value_type get_value() const noexcept     { return m_value; }

        node* get_next() const noexcept           { return m_next; }

        void set_value(value_type value) noexcept { m_value = value; }

        void set_next(node *next) noexcept        { m_next = next; }

    private:
        key_type     m_key;
        value_type   m_value;
        node*        m_next;
    };

    template <typename K> class key_hash
    {
    public:
        size_t operator()(const K &key, const unsigned int table_size) const noexcept
        {
            return (size_t)(key) % table_size;
        }
    };

    template <typename K, typename V, typename F = key_hash<K>> class hash_table
    {
        typedef F            hash_function;
        typedef V            value_type;
        typedef K            key_type;

        typedef unsigned int size_type;

        typedef       V& value_reference;
        typedef const V& const_value_reference;

        #define INITIAL_CAPACITY 16
        #define LOAD_FACTOR      0.75

    public:

        typedef       node<key_type, value_type>**       iterator;
        typedef const node<key_type, value_type>** const_iterator;

        // typedef stl::reverse_iterator<node<key_type, value_type>**>       reverse_iterator;
        // typedef stl::const_reverse_iterator<node<key_type, value_type>**> const_reverse_iterator;

        hash_table(const size_type capacity = INITIAL_CAPACITY) 
            : m_table(nullptr), m_size(0), m_capacity(capacity)
        { 
            m_table = (node<key_type, value_type>**)malloc(m_capacity * sizeof(node<key_type, value_type>*));

            if (m_table == nullptr) MALLOC_RUNTIME_ERROR

            for (size_type i = 0; i < m_capacity; i++)
                m_table[i] = 0;
        }

        void resize(size_type new_size)
        {
            node<key_type, value_type> **temp_table = (node<key_type, value_type>**)malloc(new_size * sizeof(node<key_type, value_type>*));

            if (temp_table == nullptr) MALLOC_RUNTIME_ERROR

            for (size_type i = 0; i < m_size; i++)
            {
                node<key_type, value_type> *entry = m_table[i];

                while (entry != nullptr)
                {
                    node<key_type, value_type> *next = entry->get_next();
                    size_t new_hash_value = m_hash_func(entry->get_key(), new_size);

                    entry->set_next(temp_table[new_hash_value]);

                    temp_table[new_hash_value] = entry;

                    entry = next;
                }
            }

            free(m_table);
            m_table = temp_table;
        }

        bool get(const key_type key, value_reference value)
        {
            node<key_type, value_type> *entry = m_table[m_hash_func(key, m_capacity)];

            while (entry != nullptr)
            {
                if (entry->get_key() == key)
                {
                    value = entry->get_value();
                    return true;
                }

                entry = entry->get_next();
            }

            return false;
        }

        void put(const key_type key, const value_type value)
        {
            if ((double)(m_size) / m_capacity > LOAD_FACTOR)
            {
                m_capacity *= 2;
                resize(m_capacity);
            }

            size_t hash_value = m_hash_func(key, m_capacity);

            node<key_type, value_type> *prev  = nullptr;
            node<key_type, value_type> *entry = m_table[hash_value];

            while (entry != nullptr && entry->get_key() != key)
            {
                prev = entry;
                entry = entry->get_next();
            }

            if (entry == nullptr)
            {
                entry = new node<key_type, value_type>(key, value);
                
                if (prev == nullptr)
                    m_table[hash_value] = entry;
                else
                    prev->set_next(entry);

                m_size++;
            }
            else
                entry->set_value(value);
        }

        size_type size() const noexcept     { return m_size; }

        size_type capacity() const noexcept { return m_capacity; }

        iterator table() const noexcept     { return m_table; }

        iterator begin() { return m_table; }

        iterator end()   { return m_table + m_capacity; }

        const_iterator cbegin() const noexcept { return m_table; }

        const_iterator cend() const noexcept   { return m_table + m_capacity; }

        // reverse_iterator rbegin() { return reverse_iterator(m_table + m_capacity); }

        // reverse_iterator rend()   { return reverse_iterator(m_table); }

        // const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(m_table + m_capacity); }

        // const_reverse_iterator crend() const noexcept   { return const_reverse_iterator(m_table); }

        ~hash_table() 
        {
            for (size_type i = 0; i < m_size; i++)
            {
                node<key_type, value_type> *entry = m_table[i];

                while (entry != nullptr)
                {
                    node<key_type, value_type> *temp = entry;
                    entry = entry->get_next();

                    delete temp;
                }
            }
            
            free(m_table);
        }

    private:
        node<key_type, value_type>** m_table;
        size_type                    m_size;     // total number of elements. It is incremented everytime a new <key, value> element is added
        size_type                    m_capacity; // total number of buckets
        hash_function                m_hash_func;
    };
}

#endif