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

    @file  unordered_map.h

    @brief This is part of the standard template libraries (STL) and it is used to implement an unordered map.
 */

#ifndef __UNORDERED_MAP__
#define __UNORDERED_MAP__

#include "../../iterator.h"
#include "../../allocator/allocator.h"
#include "../../functional_hash/hash.h"
#include "../../../cUtility/stl_pair.h"
#include "../../../cUtility/stl_function.h"
#include "../../../cUtility/hashable.h"

#include <malloc.h>
#include <stdexcept>
#include <initializer_list>

namespace stl
{
    /**
     * @brief This container represents a generic implementation of the hash table data structure.
     *        It consists in many member functions that help the user manage the hash table data faster and more efficiently.
     *        Moreover, it has automatic resizing when the load factor exceeds 75%.
     * @param Key        Key type
     * @param T          Value type
     * @param Hash       Hash function type
     * @param KeyEqual   Key comparison function type
     * @param Allocator  Allocator type
     */
    template <
        typename Key, 
        typename T, 
        typename Hash      = stl::hash<Key>, 
        typename KeyEqual  = stl::equal_to<Key>, 
        typename Allocator = stl::allocator<stl::__hash_node<const Key, T>>
    > class unordered_map
    {
        constexpr static unsigned short __DEFAULT_BUCKET_SIZE = 16;
        constexpr static unsigned float __DEFAULT_LOAD_FACTOR = .75;

        using allocator_traits = stl::allocator_traits<Allocator>;
        using bucket_allocator = typename Allocator::template rebind<stl::__hash_node<const Key, T>**>::other;

    public:
        typedef Key                                                         key_type;
        typedef T                                                           mapped_type;
        typedef stl::pair<const Key, T>                                     value_type;
        typedef stl::size_t                                                 size_type; 
        typedef stl::ptrdiff_t                                              difference_type;
        typedef Hash                                                        hasher;
        typedef KeyEqual                                                    key_equal;          
        typedef Allocator                                                   allocator_type;     
        typedef value_type&                                                 reference;
        typedef const value_type&                                           const_reference;
        typedef typename stl::allocator_traits<Allocator>::pointer          pointer;
        typedef typename stl::allocator_traits<Allocator>::const_pointer    const_pointer;
        typedef typename stl::__map_iterator<key_type, mapped_type>         iterator;
        typedef typename stl::__const_map_iterator<key_type, mapped_type>   const_iterator;
        typedef typename stl::__node_iterator<key_type, mapped_type>        local_iterator;
        typedef typename stl::__const_node_iterator<key_type, mapped_type>  const_local_iterator;

        unordered_map()
            : m_table(nullptr), m_size(0), m_capacity(__DEFAULT_BUCKET_SIZE), m_load_factor(__DEFAULT_LOAD_FACTOR) { } 

        unordered_map(size_type bucket_count = __DEFAULT_BUCKET_SIZE)
            : m_table(nullptr), m_size(0), m_capacity(bucket_count), m_load_factor(__DEFAULT_LOAD_FACTOR)
        { this->m_default_initialize(bucket_count); }

        unordered_map(const unordered_map &other) 
            : m_table(nullptr), m_size(0), m_capacity(other.bucket_count())
        {
            this->m_default_initialize(this->m_capacity);

            for (size_type i = 0, n = other.bucket_count(); i < n; i++)
            {
                pointer entry = other.m_table[i];

                while (entry != nullptr)
                {
                    // this->insert(entry->m_pair.first, entry->m_pair.second);
                    entry = entry->next;
                }
            }
        }

        // unordered_map(std::initializer_list<pair<key_type, value_type>> init, size_type bucket_count = INITIAL_CAPACITY) 
        //     : m_table(nullptr), m_size(0), m_capacity(bucket_count)
        // {
        //     m_table = (pair<key_type, value_type>**)m_allocator.allocate(m_capacity);

        //     if (m_table == nullptr) ALLOCATOR_RUNTIME_ERROR

        //     for (size_type i = 0; i < m_capacity; i++)
        //         m_table[i] = 0;

        //     for (pair<key_type, value_type> it : init)
        //         insert(it.get_key(), it.get_value());
        // }

        // unordered_map& operator=(const unordered_map other)
        // {
        //     if (this != &other)
        //     {
        //         m_capacity = other.bucket_count();
        //         m_size = other.size();

        //         for (size_type i = 0; i < m_capacity; i++)
        //         {
        //             pointer entry = other.get_m_table()[i];

        //             while (entry != nullptr)
        //             {
        //                 insert(entry->get_key(), entry->get_value());
        //                 entry = entry->get_next();
        //             }
        //         }   
        //     }
                     
        //     return *this;
        // }

        // unordered_map& operator=(std::initializer_list<pair<key_type, value_type>> init)
        // {
        //     m_capacity = INITIAL_CAPACITY;
        //     m_size = 0;

        //     m_table = (pair<key_type, value_type>**)m_allocator.allocate(m_capacity);

        //     if (m_table == nullptr) ALLOCATOR_RUNTIME_ERROR

        //     for (size_type i = 0; i < m_capacity; i++)
        //         m_table[i] = 0;

        //     for (pair<key_type, value_type> it : init)
        //         insert(it.get_key(), it.get_value());

        //     return *this;
        // }

        // size_type hash(key_type key)
        // {
        //     size_type hashed_key = this->m_hash(key);
        //     return hashed_key % this->m_capacity;
        // }

        // void rehash(size_type new_size)
        // {
        //     pair<key_type, value_type> **temp_table = (pair<key_type, value_type>**)m_allocator.allocate(m_capacity);

        //     if (temp_table == nullptr) ALLOCATOR_RUNTIME_ERROR

        //     for (size_type i = 0; i < new_size; i++)
        //         m_table[i] = 0;

        //     for (size_type i = 0; i < m_size; i++)
        //     {
        //         pointer entry = m_table[i];

        //         while (entry != nullptr)
        //         {
        //             pointer next = entry->get_next();
        //             size_t new_hash_value = m_hash_func(entry->get_key(), new_size);

        //             entry->set_next(temp_table[new_hash_value]);

        //             temp_table[new_hash_value] = entry;

        //             entry = next;
        //         }
        //     }

        //     m_allocator.deallocate(m_table);
        //     m_table = temp_table;
        // }

        // void reserve(size_type size)
        // {
        //     rehash(size);
        // }

        // iterator find(const key_type key)
        // {
        //     size_type hash_value = m_hash_func(key, m_capacity);
        //     pointer entry = m_table[hash_value];

        //     while (entry != nullptr)
        //     {
        //         if (m_key_equal(entry->get_key(), key))
        //             return iterator(m_table, m_table + m_capacity, entry);

        //         entry = entry->get_next();
        //     }

        //     return end();
        // }

        // const_iterator find(const key_type key) const
        // {
        //     size_type hash_value = m_hash_func(key, m_capacity);
        //     pointer entry = m_table[hash_value];

        //     while (entry != nullptr)
        //     {
        //         if (m_key_equal(entry->get_key(), key))
        //             return const_iterator(m_table, m_table + m_capacity, entry);

        //         entry = entry->get_next();
        //     }

        //     return cend();
        // }

        // bool contains(const key_type key)
        // {
        //     pointer entry = m_table[m_hash_func(key, m_capacity)];

        //     while (entry != nullptr)
        //     {
        //         if (m_key_equal(entry->get_key(), key))
        //             return true;

        //         entry = entry->get_next();
        //     }

        //     return false;
        // }

        // void clear() noexcept
        // {
        //     for (size_type i = 0; i < m_capacity; ++i)
        //     {
        //         pointer entry = m_table[i];

        //         while (entry != nullptr)
        //         {
        //             pointer temp = entry;
        //             entry = entry->get_next();

        //             delete temp;
        //         }

        //         m_table[i] = nullptr; 
        //     }

        //     m_size = 0;
        // }

        // pair<iterator, bool> insert(const pair<key_type, value_type> p)
        // { 
        //     if ((double)(m_size) / m_capacity > LOAD_FACTOR)
        //     {
        //         m_capacity *= 2;
        //         rehash(m_capacity);
        //     }

        //     value_type pair_value = p.get_value();
        //     key_type pair_key     = p.get_key();
        
        //     size_type hash_value = m_hash_func(pair_key, m_capacity);

        //     pointer prev = nullptr;
        //     pointer entry = m_table[hash_value];

        //     while (entry != nullptr && !m_key_equal(entry->get_key(), pair_key))
        //     {
        //         prev = entry;
        //         entry = entry->get_next();
        //     }

        //     if (entry == nullptr)
        //     {
        //         entry = new pair<key_type, value_type>(pair_key, pair_value);

        //         if (prev == nullptr)
        //             m_table[hash_value] = entry;
        //         else
        //             prev->set_next(entry);

        //         m_size++;

        //         return {iterator(m_table, m_table + m_capacity, entry), true};
        //     }
        //     else
        //         entry->set_value(pair_value);

        //     return {iterator(m_table, m_table + m_capacity, entry), false};
        // }

        // iterator insert(const_iterator hint, const pair<key_type, value_type> p)
        // {
        //     pointer entry = hint.m_node_it;

        //     key_type entry_key     = entry->get_key();
        //     value_type entry_value = entry->get_value();

        //     key_type pair_key     = p.get_key();
        //     value_type pair_value = p.get_value();

        //     pointer prev = nullptr;

        //     if (m_key_equal(entry_key, pair_key))
        //         entry->set_value(pair_value);
        //     else
        //     {
        //         while (entry != nullptr)
        //         {
        //             prev = entry;
        //             entry = entry->get_next();
        //         }

        //         entry = new pair<key_type, value_type>(pair_key, pair_value);

        //         prev->set_next(entry);
        //     }

        //     return iterator(m_table, m_table + m_capacity, entry);
        // }

        // void insert(std::initializer_list<pair<key_type, value_type>> init)
        // {
        //     if ((double)(m_size) / m_capacity > LOAD_FACTOR)
        //     {
        //         m_capacity *= 2;
        //         rehash(m_capacity);
        //     }

        //     for (pair<key_type, value_type> it : init)
        //     {
        //         key_type pair_key = it.get_key();

        //         size_type hash_value = m_hash_func(pair_key, m_capacity);

        //         pointer prev = nullptr;
        //         pointer entry = m_table[hash_value];

        //         while (entry != nullptr && !m_key_equal(entry->get_key(), pair_key))
        //         {
        //             prev = entry;
        //             entry = entry->get_next();
        //         }

        //         if (entry == nullptr)
        //         {
        //             entry = new pair<key_type, value_type>(pair_key, it.get_value());
                    
        //             if (prev == nullptr)
        //                 m_table[hash_value] = entry;
        //             else
        //                 prev->set_next(entry);

        //             m_size++;
        //         }
        //         else
        //             entry->set_value(it.get_value());
        //     }
        // }

        // void insert(const key_type key, const value_type value)
        // {
        //     // check the load factor to see if the hash table needs resizing. (to do: not sure this is the right approach) 
        //     // LOAD_FACTOR = 75% (predefined)
        //     if ((double)(m_size) / m_capacity > LOAD_FACTOR)
        //     {
        //         m_capacity *= 2;
        //         rehash(m_capacity);
        //     }

        //     size_type hash_value = m_hash_func(key, m_capacity);

        //     pointer prev  = nullptr;             // Keeps track of the last found pair in the forward list when the while is terminated
        //     pointer entry = m_table[hash_value]; // select the entry point for the forward list

        //     // transverse the forward list to the end while looking for the key
        //     while (entry != nullptr && !m_key_equal(entry->get_key(), key))
        //     {
        //         prev = entry;
        //         entry = entry->get_next();
        //     }
            
        //     // if the entry is null that means the end of the list was reached and the key was not found
        //     if (entry == nullptr)
        //     {
        //         // create a new pair for the new element
        //         entry = new pair<key_type, value_type>(key, value);
                
        //         // if prev is null that means the entry was null from the begining and there
        //         // is not a list currently at that specific hash value. Therefore that bucket is assigned the address of the entry
        //         if (prev == nullptr)
        //             m_table[hash_value] = entry;
        //         else
        //             // if prev != null that means that there is a list at the hash value and the entry is added at the back of this list
        //             prev->set_next(entry);

        //         // m_size gets incremented for each entry
        //         m_size++;
        //     }
        //     else
        //         // if entry is not null that means the prev while was terminated when the same key was found.
        //         // Therefore the value is just updated for this specific key
        //         entry->set_value(value);
        // }

        // bool empty() const noexcept
        // {
        //     for (size_type i = 0; i < m_capacity; i++)
        //         if (m_table[i] != nullptr)
        //             return false;

        //     return true;
        // }

        // pair<key_type, value_type>** get_m_table() const noexcept { return m_table; }

        // size_type size() const noexcept         { return m_size; }

        // size_type bucket_count() const noexcept { return m_capacity; }

        // size_type bucket_size(size_type index) const
        // {
        //     size_type count = 0;
        //     pointer entry = m_table[index];

        //     while (entry != nullptr)
        //     {
        //         count++;
        //         entry = entry->get_next();
        //     }

        //     return count;
        // }

        // size_type bucket(const key_type key)
        // {   
        //     return m_hash_func(key) % m_capacity;
        // }

        // value_reference at(const key_type key)
        // {
        //     pointer entry = m_table[m_hash_func(key, m_capacity)];

        //     while (entry != nullptr)
        //     {   
        //         if (m_key_equal(entry->get_key(), key))
        //             return entry->get_value();

        //         entry = entry->get_next();
        //     }

        //     throw std::out_of_range("Key not found!\n");
        // }

        // value_type& operator[](const key_type key)
        // {
        //     for (size_type i = 0; i < m_capacity; i++)
        //     {
        //         pointer entry = m_table[i];

        //         while (entry != nullptr)
        //         {
        //             if (m_key_equal(entry->get_key(), key))
        //                 return entry->get_value();

        //             entry = entry->get_next();
        //         }
        //     }

        //     throw std::out_of_range("Key not found!\n"); 
        // }

        // iterator begin() { return iterator(m_table, m_table + m_capacity, m_table[0]); }

        // iterator end()   { return iterator(m_table + m_capacity, m_table + m_capacity, nullptr); }

        // const_iterator cbegin() const noexcept { return const_iterator(m_table, m_table + m_capacity, m_table[0]); }

        // const_iterator cend() const noexcept   { return const_iterator(m_table + m_capacity, m_table + m_capacity, nullptr); }

        // bucket_reverse_iterator rbegin() { return bucket_reverse_iterator(m_table + m_capacity); }

        // bucket_reverse_iterator rend()   { return bucket_reverse_iterator(m_table); }

        // const_bucket_reverse_iterator crbegin() const noexcept { return const_bucket_reverse_iterator(m_table + m_capacity); }

        // const_bucket_reverse_iterator crend() const noexcept   { return const_bucket_reverse_iterator(m_table); }

        // ~unordered_map() 
        // {
        //     for (size_type i = 0; i < m_size; i++)
        //     {
        //         pointer entry = m_table[i];

        //         while (entry != nullptr)
        //         {
        //             pointer temp = entry;
        //             entry = entry->get_next();

        //             m_allocator.n_deallocate(temp); // the nodes are allocated using the new operator
        //         }
        //     }
            
        //     m_allocator.deallocate(m_table);
        // }

    private:
        value_type*     m_table;
        size_type       m_size;             // total number of elements. It is incremented everytime a new <key, value> element is added
        size_type       m_capacity;         // total number of buckets
        float           m_load_factor;
        hasher          m_hash;
        key_equal       m_key_equal;
        allocator_type  m_alloc;

        value_type* m_get_table(const size_type bucket_count);

        void m_default_initialize(const size_type bucket_count);
    };
}

#include "unordered_map.tcc"

#endif // HASH_MAP_H