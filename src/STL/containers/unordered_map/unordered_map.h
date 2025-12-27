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

#include <stdexcept>
#include <initializer_list>

namespace stl
{
    /**
     * @brief This container represents a generic implementation of the hash table data structure.
     *        It consists in many member functions that help the user manage the hash table data faster and more efficiently.
     *        Moreover, it has automatic rehashing when the load factor exceeds 75%.
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
        typename Allocator = stl::allocator<stl::pair_node<Key, T>>
    > class unordered_map
    {
        constexpr static unsigned short   __DEFAULT_BUCKET_SIZE = 16;
        constexpr static float            __DEFAULT_LOAD_FACTOR = .75;

        using allocator_traits = stl::allocator_traits<Allocator>;
        using bucket_allocator = typename Allocator::template rebind<stl::pair_node<Key, T>*>::other;

    public:
        typedef Key                                                         key_type;
        typedef T                                                           mapped_type;
        typedef stl::pair_node<Key, T>                                      value_type;
        typedef stl::size_t                                                 size_type; 
        typedef stl::ptrdiff_t                                              difference_type;
        typedef Hash                                                        hasher;
        typedef KeyEqual                                                    key_equal;          
        typedef Allocator                                                   allocator_type;     
        typedef value_type&                                                 reference;
        typedef const value_type&                                           const_reference;
        typedef typename allocator_traits::pointer                          pointer;
        typedef typename allocator_traits::const_pointer                    const_pointer;
        typedef typename stl::map_iterator<key_type, mapped_type>           iterator;
        typedef typename stl::const_map_iterator<key_type, mapped_type>     const_iterator;
        typedef typename stl::node_iterator<key_type, mapped_type>          local_iterator;
        typedef typename stl::const_node_iterator<key_type, mapped_type>    const_local_iterator;
        
        struct node_type;

        unordered_map()
            : m_table(nullptr), m_size(0), m_capacity(__DEFAULT_BUCKET_SIZE), m_load_factor(__DEFAULT_LOAD_FACTOR), m_hash(), m_key_equal(), m_alloc()
        { this->m_default_initialize(__DEFAULT_BUCKET_SIZE); } 

        explicit unordered_map(size_type bucket_count, const hasher& hash = Hash(), const key_equal& equal = KeyEqual(), const allocator_type& alloc = Allocator())
            : m_table(nullptr), m_size(0), m_capacity(bucket_count), m_load_factor(__DEFAULT_LOAD_FACTOR), m_hash(hash), m_key_equal(equal), m_alloc(alloc)
        { this->m_default_initialize(bucket_count); }

        unordered_map(size_type bucket_count, const allocator_type& alloc)
            : unordered_map(bucket_count, Hash(), KeyEqual(), alloc) { }

        unordered_map(size_type bucket_count, const hasher& hash, const allocator_type& alloc)
            : unordered_map(bucket_count, hash, KeyEqual(), alloc) { }

        explicit unordered_map(const allocator_type& alloc)
            : unordered_map(__DEFAULT_BUCKET_SIZE, Hash(), KeyEqual(), alloc) { }

        template <typename InputIt, typename = stl::RequireIterator<InputIt>>
        unordered_map(InputIt first, InputIt last, size_type bucket_count = __DEFAULT_BUCKET_SIZE, const hasher& hash = Hash(), const key_equal& equal = KeyEqual(), const allocator_type& alloc = Allocator())
            : unordered_map(bucket_count, hash, equal, alloc)
        { this->m_range_initialize(first, last); }
        
        template <typename InputIt, typename = stl::RequireIterator<InputIt>>
        unordered_map(InputIt first, InputIt last, size_type bucket_count, const allocator_type& alloc)
            : unordered_map(first, last, bucket_count, Hash(), KeyEqual(), alloc) { }

        template <typename InputIt, typename = stl::RequireIterator<InputIt>>
        unordered_map(InputIt first, InputIt last, size_type bucket_count, const hasher& hash, const allocator_type& alloc)
            : unordered_map(first, last, bucket_count, hash, KeyEqual(), alloc) { }

        unordered_map(const unordered_map& other) 
            : m_table(nullptr), m_size(0), m_capacity(other.m_capacity), m_load_factor(other.m_load_factor),
              m_hash(other.m_hash), m_key_equal(other.m_key_equal), m_alloc(allocator_traits::select_on_container_copy_construction(other.m_alloc))
        { this->m_range_initialize(other.cbegin(), other.cend(), this->m_capacity); }

        unordered_map(const unordered_map& other, const allocator_type& alloc)
            : m_table(nullptr), m_size(0), m_capacity(other.m_capacity), m_load_factor(other.m_load_factor),
              m_hash(other.m_hash), m_key_equal(other.m_key_equal), m_alloc(alloc)
        { this->m_range_initialize(other.cbegin(), other.cend(), this->m_capacity); }

        unordered_map(const unordered_map&& other) 
            : m_table(other.m_table), m_size(other.m_size), m_capacity(other.m_capacity), m_load_factor(other.m_load_factor),
              m_hash(stl::move(other.m_hash)), m_key_equal(stl::move(other.m_key_equal)), m_alloc(stl::move(other.m_alloc)) 
        { 
            other.m_size = other.m_capacity = 0;
            other.m_table = nullptr;
        }

        unordered_map(const unordered_map&& other, const allocator_type& alloc)
            : m_table(other.m_table), m_size(other.m_size), m_capacity(other.m_capacity), m_load_factor(other.m_load_factor),
              m_hash(stl::move(other.m_hash)), m_key_equal(stl::move(other.m_key_equal)), m_alloc(alloc) 
        { 
            other.m_size = other.m_capacity = 0;
            other.m_table = nullptr;
        }

        unordered_map(std::initializer_list<value_type> ilist, size_type bucket_count = __DEFAULT_BUCKET_SIZE, const hasher& hash = Hash(), const key_equal& equal = KeyEqual(), const allocator_type& alloc = Allocator())
            : m_table(nullptr), m_size(0), m_capacity(bucket_count), m_load_factor(__DEFAULT_LOAD_FACTOR), m_hash(hash), m_key_equal(equal), m_alloc(alloc)
        { this->m_range_initialize(ilist.begin(), ilist.end(), bucket_count); }

        unordered_map(std::initializer_list<value_type> ilist, size_type bucket_count, const allocator_type& alloc)
            : unordered_map(ilist, bucket_count, Hash(), KeyEqual(), alloc) { }

        unordered_map(std::initializer_list<value_type> ilist, size_type bucket_count, const hasher& hash, const allocator_type& alloc)
            : unordered_map(ilist, bucket_count, hash, KeyEqual(), alloc) { }
        
        ~unordered_map()
        { this->m_destroy_table(); }

        unordered_map& operator=(const unordered_map& other)
        {
            if (this != &other)
            {
                if (this->m_table != nullptr)
                {
                    if (this->m_size > 0)
                        this->clear();

                    this->m_check_rehash(other.m_size, this->m_capacity, this->m_load_factor);
                }

                this->m_size = other.m_size;
                this->m_capacity = other.m_capacity;
                this->m_load_factor = other.m_load_factor;
                this->m_hash = other.m_hash;
                this->m_key_equal = other.m_key_equal;
                this->m_alloc = allocator_traits::select_on_container_copy_construction(other.m_alloc);
                this->m_range_initialize(other.cbegin(), other.cend(), this->m_capacity);
                
                other.m_size = other.m_capacity = 0;
                other.m_table = nullptr;
            }

            return *this;
        }

        unordered_map& operator=(unordered_map&& other) noexcept
        {
            if (this != &other)
            {
                if (this->m_table != nullptr)      
                    this->m_destroy_table();

                this->m_table = other.m_table;
                this->m_size = other.m_size;
                this->m_capacity = other.m_capacity;
                this->m_load_factor = other.m_load_factor;
                this->m_hash = stl::move(other.m_hash);
                this->m_key_equal = stl::move(other.m_key_equal);
                this->m_alloc = stl::move(other.m_alloc);
            }

            return *this;
        }

        unordered_map& operator=(std::initializer_list<value_type> ilist)
        {
            if (this->m_table != nullptr)
            {
                if (this->m_size > 0)
                    this->clear();

                this->m_check_rehash(ilist.size(), this->m_capacity, this->m_load_factor);
            }

            this->m_table = nullptr;
            this->m_size = 0;
            this->m_capacity = __DEFAULT_BUCKET_SIZE;
            this->m_load_factor = __DEFAULT_LOAD_FACTOR;
            this->m_range_initialize(ilist.begin(), ilist.end(), this->m_capacity);

            return *this;
        }

        allocator_type get_allocator() const noexcept 
        { return this->m_alloc; }

        iterator begin() { return iterator(this->m_table, this->m_table + this->m_capacity, *this->m_table); }

        iterator end() { return iterator(this->m_table + this->m_capacity, this->m_table + this->m_capacity, nullptr); }

        const_iterator cbegin() const noexcept { return const_iterator(this->m_table, this->m_table + this->m_capacity, *this->m_table); }

        const_iterator cend() const noexcept { return const_iterator(this->m_table + this->m_capacity, this->m_table + this->m_capacity, nullptr); }

        bool empty() const noexcept 
        { return this->m_size == 0; }

        size_type size() const noexcept 
        { return this->m_size; }

        size_type max_size() const noexcept 
        { return std::numeric_limits<difference_type>::max(); }

        void clear() noexcept;

        pair<iterator, bool> insert(const_reference node)
        { return this->m_insert(node.m_pair.first, node.m_pair.second); }

        pair<iterator, bool> insert(const key_type key, const value_type value)
        { return this->m_insert(key, value); }

        pair<iterator, bool> insert(value_type&& node)
        { return this->m_insert(stl::move(node.m_pair.first), stl::move(node.m_pair.second)); }

        iterator insert(const_iterator hint, const_reference node)
        { return this->m_insert(hint, node.m_pair.first, node.m_pair.second); }

        iterator insert(const_iterator hint, value_type&& node)
        { return this->insert(hint, stl::move(node.m_pair.first), stl::move(node.m_pair.second)); }

        template <typename InputIt, typename = stl::RequireIterator<InputIt>>
        void insert(InputIt first, InputIt last);

        void insert(std::initializer_list<value_type> ilist)
        { this->insert(ilist.begin(), ilist.end()); }

        template <typename... Args>
        pair<iterator, bool> emplace(Args&&... args)
        { return this->m_insert(stl::forward<Args>(args)...); }

        template <typename... Args>
        iterator emplace_hint(const_iterator hint, Args&&... args)
        { return this->m_insert(hint, stl::forward<Args>(args)...); }

        template <typename... Args>
        pair<iterator, bool> try_emplace(const key_type& key, Args&&... args)
        { return this->m_try_emplace(key, stl::forward<Args>(args)...); }

        template <typename... Args>
        pair<iterator, bool> try_emplace(key_type&& key, Args&&... args)
        { return this->m_try_emplace(stl::move(key), stl::forward<Args>(args)...); }

        template <typename K, typename... Args>
        pair<iterator, bool> try_emplace(K&& key, Args&&... args)
        { return this->m_try_emplace(stl::forward<K>(key), stl::forward<Args>(args)...); }

        template <typename... Args>
        iterator try_emplace(const_iterator hint, const key_type& key, Args&&... args)
        { return this->m_try_emplace(hint, key, stl::forward<Args>(args)...); }

        template <typename... Args>
        iterator try_emplace(const_iterator hint, key_type&& key, Args&&... args)
        { return this->m_try_emplace(hint, stl::move(key), stl::forward<Args>(args)...); }

        template <typename K, typename... Args>
        iterator try_emplace(const_iterator hint, K&& key, Args&&... args)
        { return this->try_emplace(hint, stl::forward<K>(key), stl::forward<Args>(args)...); }

        iterator erase(const_iterator first, const_iterator last);

        iterator erase(iterator pos);

        iterator erase(const_iterator pos)
        { return this->erase(pos, stl::next(pos)); }

        size_type erase(const key_type& key)
        {
            iterator it = this->find(key);

            if (it != this->end())
            {
                this->erase(it);
                return 1;
            }

            return 0;
        }

        template <typename K>
        size_type erase(K&& x);

        void swap(unordered_map& other) noexcept
        {
            stl::swap(this->m_table, other.m_table);
            stl::swap(this->m_capacity, other.m_capacity);
            stl::swap(this->m_size, other.m_size);
            stl::swap(this->m_hash, other.m_hash);
            stl::swap(this->m_key_equal, other.m_key_equal);
            stl::swap(this->m_alloc, other.m_alloc);
        }

        node_type extract(const_iterator pos)
        { return this->extract(pos.m_current->m_pair.first); }

        node_type extract(const key_type& key);

        template <typename K>
        node_type extract(K&& x);

        constexpr pointer* table() const noexcept { return this->m_table; }

        // let this func update cap
        // old cap used for rehashing
        void rehash(size_type new_size);

        void reserve(size_type count)
        {
            size_type new_cap = static_cast<size_type>(count / this->m_load_factor);
            new_cap = (1 > new_cap) ? 1 : new_cap; // Ensure at least 1 bucket exists
            this->rehash(new_cap);
        }

        iterator find(const key_type key)
        {
            pointer entry = this->m_table[this->hash(key)];

            while (entry != nullptr)
            {
                if (this->m_key_equal(entry->m_pair.first, key))
                    return iterator(this->m_table, this->m_table + this->m_capacity, entry);

                entry = entry->m_next;
            }

            return this->end();
        }

        const_iterator find(const key_type key) const
        {
            pointer entry = this->m_table[this->hash(key)];

            while (entry != nullptr)
            {
                if (this->m_key_equal(entry->m_pair.first, key))
                    return const_iterator(this->m_table, this->m_table + this->m_capacity, entry);

                entry = entry->m_next;
            }

            return this->cend();
        }

        bool contains(const key_type key)
        {
            pointer entry = this->m_table[this->hash(key)];

            while (entry != nullptr)
            {
                if (this->m_key_equal(entry->m_pair.first, key))
                    return true;

                entry = entry->m_next;
            }

            return false;
        }

        size_type bucket_count() const noexcept { return m_capacity; }

        size_type bucket_size(size_type index) const
        {
            size_type count = 0;
            pointer entry = this->m_table[index];

            while (entry != nullptr)
            {
                ++count;
                entry = entry->m_next;
            }

            return count;
        }

        size_type bucket(const key_type key) const
        { return this->hash(key); }

        mapped_type& at(const key_type key)
        {
            iterator it = this->find(key);

            if (it == this->end())
                throw std::out_of_range("Key not found!\n");

            return it->second;
        }

        const mapped_type& at(const key_type& key) const
        {  
            const_iterator cit = this->find(key);

            if (cit == this->cend())
                throw std::out_of_range("Key not found!\n");

            return cit->second;
        }

        template <typename K>
        mapped_type& at(const K& x)
        {
            iterator it = this->find(x);

            if (it == this->end())
                throw std::out_of_range("Key not found!\n");

            return it->second;
        }

        template <typename K>
        const mapped_type& at(const K& x) const
        {
            const_iterator cit = this->find(x);

            if (cit == this->cend())
                throw std::out_of_range("Key not found!\n");

            return cit->second;
        }

        mapped_type& operator[](const key_type& key)
        { return this->try_emplace(key).first->second; }

        mapped_type& operator[](key_type&& key)
        { return this->try_emplace(stl::move(key)).first->second; }

        template <typename K>
        mapped_type& operator[](K&& x)
        { return this->try_emplace(stl::forward<K>(x)).first->second; }

        size_type count(const key_type& key) const
        {
            iterator it = this->find(key);

            if (it == this->end())
                return 0;

            return 1;
        }

        template <typename K>
        size_type count(const K& x) const
        {
            iterator it = this->find(stl::forward<K>(x));

            if (it == this->end())
                return 0;

            return 1;
        }

        local_iterator begin(size_type n) 
        { 
            if (n >= this->m_capacity)
                throw std::out_of_range("Index out of bounds!\n");

            return local_iterator(*(this->m_table + n));
        }

        local_iterator end(size_type n)
        {
            if (n >= this->m_capacity)
                throw std::out_of_range("Index out of bounds!\n");

            return local_iterator(*(this->m_table + n));
        }

        const_local_iterator cbegin(size_type n) const
        {
            if (n >= this->m_capacity)
                throw std::out_of_range("Index out of bounds!\n");

            return const_local_iterator(*(this->m_table + n));
        }

        const_local_iterator cend(size_type n) const
        {
            if (n >= this->m_capacity)
                throw std::out_of_range("Index out of bounds!\n");

            return const_local_iterator(*(this->m_table + n));
        }

    private:
        pointer*        m_table;
        size_type       m_size;             // total number of elements. It is incremented everytime a new <key, value> element is added
        size_type       m_capacity;         // total number of buckets
        float           m_load_factor;
        hasher          m_hash;
        key_equal       m_key_equal;
        allocator_type  m_alloc;

        size_type hash(const key_type key)
        { return static_cast<size_type>(this->m_hash(key) % this->m_capacity); }

        pointer* m_get_table(const size_type bucket_count);

        pointer m_get_node(key_type&& key, mapped_type&& value);

        void m_deallocate_table();

        void m_destroy_table();

        void m_default_initialize(const size_type bucket_count);

        template <typename InputIt>
        void m_range_initialize(InputIt first, InputIt last, size_type bucket_count = __DEFAULT_BUCKET_SIZE);

        void m_range_initialize(const_iterator first, const_iterator last, size_type bucket_count = __DEFAULT_BUCKET_SIZE);

        void m_check_rehash(size_type t_size, size_type b_size, float load_factor = __DEFAULT_LOAD_FACTOR);

        template <typename... Args>
        pair<iterator, bool> m_insert(Args&&... args);

        template <typename... Args>
        iterator m_insert(const_iterator hint, Args&&... args);
        
        template <typename... Args>
        pair<iterator, bool> m_try_emplace(const key_type& key, Args&&... args);

        template <typename... Args>
        iterator m_try_emplace(const_iterator hint, const key_type& key, Args&&... args);
    };
}

#include "unordered_map.tcc"

#endif // HASH_MAP_H    