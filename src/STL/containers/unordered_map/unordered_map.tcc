namespace stl
{
    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    class unordered_map<Key, T, Hash, KeyEqual, Allocator>::node_type
    {
        node_type(pair_node<Key, T>* node = nullptr, const Allocator& alloc = Allocator())
            : m_node(node), m_alloc(alloc) { }

        node_type(node_type&& other) noexcept
            : m_node(other.m_node)
        { other.m_node = nullptr; }

        ~node_type()
        {
            if (this->m_node != nullptr)
            {
                this->m_alloc.destroy(this->m_node);
                this->m_alloc.deallocate(this->m_node, 1);
                this->m_node = nullptr;
            }
        }

        node_type& operator=(node_type&& other) noexcept
        {
            if (this != &other)
            {
                if (this->m_node != other.m_node && this->m_node != nullptr)
                {
                    this->m_alloc.destroy(this->m_node);
                    this->m_alloc.deallocate(this->m_node, 1);
                }

                this->m_node = other.m_node;
                this->m_alloc = stl::move(other.m_alloc);
                other.m_node = nullptr;
            }

            return *this;
        }

        const Key& key() const { return this->m_node->m_pair.first; }

        T& mapped() { return this->m_node->m_pair.second; }

        bool empty() const { return this->m_node == nullptr; }

    private:
        pair_node<Key, T>* m_node;
        Allocator          m_alloc;

        node_type(const node_type&) = delete;
        node_type& operator=(const node_type&) = delete; 
    };

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    void unordered_map<Key, T, Hash, KeyEqual, Allocator>::clear() noexcept
    {
        for (size_type i = 0; i < this->m_capacity; ++i)
        {
            pointer entry = this->m_table[i];

            while (entry != nullptr)
            {
                pointer temp = entry;
                entry = entry->m_next;

                this->m_alloc.destroy(temp);
                this->m_alloc.deallocate(temp, 1);
            }

            this->m_table[i] = nullptr;
        }
        
        this->m_size = 0;
    }

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    template <typename InputIt, typename>
    void unordered_map<Key, T, Hash, KeyEqual, Allocator>::insert(InputIt first, InputIt last)
    {
        for (; first != last; ++first)
            this->insert(*first);
    }

    /**
     * Try to improve design
     */
    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    typename unordered_map<Key, T, Hash, KeyEqual, Allocator>::iterator 
    unordered_map<Key, T, Hash, KeyEqual, Allocator>::erase(const_iterator first, const_iterator last)
    {
        if (first == last)
            return iterator(this->m_table, this->m_table + this->m_capacity, last);

        const_iterator prev;
        bool is_list_head = true;

        for (const_iterator cit = this->cbegin(); cit != this->cend(); ++cit)
            if (cit.m_current->m_next == first.m_current)
            {
                is_list_head = false;
                prev = cit;
                break;
            }

        if (is_list_head)
        {
            size_type hash_value = this->hash(first.m_current->m_pair.first);
            this->m_table[hash_value] = last.m_current;
        }
        else
            prev.m_current->m_next = last.m_current;

        const_iterator current = first;
        while (current != last)
        {
            const_iterator next = current + 1;

            this->m_alloc.destroy(current.m_current);
            this->m_alloc.deallocate(current.m_current, 1);

            current = next;
        }

        return iterator(this->m_table, this->m_table + this->m_capacity, last);
    }

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    typename unordered_map<Key, T, Hash, KeyEqual, Allocator>::iterator
    unordered_map<Key, T, Hash, KeyEqual, Allocator>::erase(iterator pos)
    {
        if (pos.m_current == nullptr)
            return iterator(this->m_table, this->m_table + this->m_capacity, nullptr);

        size_type hash_value = this->hash(pos.m_current->m_pair.first);
        pointer entry = this->m_table[hash_value], prev = nullptr;

        while (entry != nullptr)
        {
            if (this->m_key_equal(entry->m_pair.first, pos.m_current->m_pair.first))
            {
                if (prev == nullptr)
                    this->m_table[hash_value] = entry->m_next;
                else
                    prev->m_next = entry->m_next;

                this->m_alloc.destroy(entry);
                this->m_alloc.deallocate(entry, 1);

                return iterator(this->m_table, this->m_table + this->m_capacity, (prev == nullptr) ? this->m_table[hash_value] : prev->m_next);
            }

            prev = entry;
            entry = entry->m_next;
        }

        return iterator(this->m_table, this->m_table + this->m_capacity, nullptr);
    }

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    template <typename K>
    typename unordered_map<Key, T, Hash, KeyEqual, Allocator>::size_type
    unordered_map<Key, T, Hash, KeyEqual, Allocator>::erase(K&& x)
    {
        pointer entry = this->m_table[this->hash(x)];

        while (entry != nullptr)
        {
            pointer next = entry->m_next;

            if (this->m_key_equal(entry->m_pair.first, stl::forward<K>(x)))
            {
                this->erase(iterator(this->m_table, this->m_table + this->m_capacity, entry));
                return 1;
            }

            entry = next;
        }

        return 0;
    }

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    void unordered_map<Key, T, Hash, KeyEqual, Allocator>::rehash(size_type new_size)
    {
        size_type old_cap = this->m_capacity;
        pointer* temp = this->m_get_table(new_size);

        for (size_type i = 0; i < new_size; ++i)
            *(temp + i) = nullptr;

        for (size_type i = 0; i < old_cap; ++i)
        {
            pointer entry = this->m_table[i];

            while (entry != nullptr)
            {
                pointer next = entry->m_next;
                size_type new_hash = this->hash(entry->m_pair.first);

                entry->m_next = temp[new_hash];
                temp[new_hash] = entry;
                entry = next;
            }
        }
        
        this->m_deallocate_table();

        this->m_capacity = new_size;
        this->m_table = temp;
    }

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    typename unordered_map<Key, T, Hash, KeyEqual, Allocator>::node_type
    unordered_map<Key, T, Hash, KeyEqual, Allocator>::extract(const key_type& key)
    {
        size_type hash_value = this->hash(key);
        pointer entry = this->m_table[hash_value], prev = nullptr;

        while (entry != nullptr)
        {
            if (this->m_key_equal(entry->m_pair.first, key))
            {
                if (prev == nullptr)
                    this->m_table[hash_value] = entry->m_next;
                else
                    prev->m_next = entry->m_next;
                
                entry->m_next = nullptr;
                --this->m_size;
                
                return node_type(entry, this->m_alloc);
            }

            prev = entry;
            entry = entry->m_next;
        }

        return node_type();
    }

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    template <typename K>
    typename unordered_map<Key, T, Hash, KeyEqual, Allocator>::node_type
    unordered_map<Key, T, Hash, KeyEqual, Allocator>::extract(K&& x)
    {
        size_type hash_value = this->hash(x);
        pointer entry = this->m_table[hash_value], prev = nullptr;

        while (entry != nullptr)
        {
            if (this->m_key_equal(entry->m_next.first, stl::forward<K>(x)))
            {
                if (prev == nullptr)
                    this->m_table[hash_value] = entry->m_next;
                else
                    prev->m_next = entry->m_next;

                return node_type(entry);
            }

            prev = entry;
            entry = entry->m_next;
        }

        return node_type();
    }

    /// @c private_members

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    typename unordered_map<Key, T, Hash, KeyEqual, Allocator>::pointer*
    unordered_map<Key, T, Hash, KeyEqual, Allocator>::m_get_table(const size_type bucket_count)
    {
        bucket_allocator __alloc = this->m_alloc;
        pointer* __temp = __alloc.allocate(bucket_count);
        return __temp; 
    }

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    typename unordered_map<Key, T, Hash, KeyEqual, Allocator>::pointer
    unordered_map<Key, T, Hash, KeyEqual, Allocator>::m_get_node(key_type&& key, mapped_type&& value)
    {
        pointer __new_node = this->m_alloc.allocate(1);

        new (__new_node) stl::pair_node<key_type, mapped_type>(stl::forward<key_type>(key), stl::forward<mapped_type>(value));
        __new_node->m_next = nullptr;

        return __new_node;
    }

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    void unordered_map<Key, T, Hash, KeyEqual, Allocator>::m_default_initialize(const size_type bucket_count)
    {
        this->m_capacity = bucket_count;
        this->m_table = this->m_get_table(bucket_count);

        for (size_type i = 0; i < bucket_count; ++i)
            *(this->m_table + i) = nullptr;
    }

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    void unordered_map<Key, T, Hash, KeyEqual, Allocator>::m_deallocate_table()
    {
        bucket_allocator __alloc = this->m_alloc;
        __alloc.deallocate(this->m_table, this->m_capacity);
        this->m_table = nullptr;
    }

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    void unordered_map<Key, T, Hash, KeyEqual, Allocator>::m_destroy_table()
    {
        this->clear();
        this->m_deallocate_table();
        this->m_capacity = 0;
    }

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    template <typename InputIt>
    void unordered_map<Key, T, Hash, KeyEqual, Allocator>::m_range_initialize(InputIt first, InputIt last, size_type bucket_count)
    {
        this->m_default_initialize(bucket_count);
        
        for (; first != last; ++first)
            this->insert(*first);
    }

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    void unordered_map<Key, T, Hash, KeyEqual, Allocator>::m_range_initialize(const_iterator first, const_iterator last, size_type bucket_count)
    {
        this->m_default_initialize(bucket_count);

        for (; first != last; ++first)
            this->insert(first.m_current);
    }

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    void unordered_map<Key, T, Hash, KeyEqual, Allocator>::m_check_rehash(size_type t_size, size_type b_size, float load_factor)
    {
        if (static_cast<float>(t_size) / b_size > load_factor)
        {
            size_type new_cap = this->m_capacity + this->m_capacity / 2 + 1;
            this->rehash(new_cap);
        }
    }

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    template <typename... Args>
    pair<typename unordered_map<Key, T, Hash, KeyEqual, Allocator>::iterator, bool>
    unordered_map<Key, T, Hash, KeyEqual, Allocator>::m_insert(Args&&... args)
    {
        // check the load factor to see if the hash table needs resizing.
        this->m_check_rehash(this->m_size, this->m_capacity, this->m_load_factor);
        
        // construct the node using perfect forwarding 
        stl::pair_node<key_type, mapped_type> node(stl::forward<Args>(args)...);

        key_type key = node.m_pair.first;
        mapped_type value = node.m_pair.second;

        size_type hash_value = this->hash(key);
    
        pointer prev = nullptr; // Keeps track of the last found pair in the forward list when the while is terminated              
        pointer entry = *(this->m_table + hash_value); // select the entry point for the forward list

        // transverse the forward list to the end while looking for the key
        while (entry != nullptr && !this->m_key_equal(entry->m_pair.first, key))
        {
            prev = entry;
            entry = entry->m_next;
        }

        // if the entry is null that means the end of the list was reached and the key was not found
        if (entry == nullptr)
        {
            // create a new pair for the new element
            entry = this->m_get_node(stl::move(key), stl::move(value));
            
            // if prev is null that means the entry was null from the begining and there
            // is not a list currently at that specific hash value. Therefore that bucket is assigned the address of the entry
            if (prev == nullptr)
                this->m_table[hash_value] = entry;
            else
                 // if prev != null that means that there is a list at the hash value and the entry is added at the back of this list
                prev->m_next = entry;

            // m_size gets incremented for each entry
            ++this->m_size;

            return {iterator(this->m_table, this->m_table + this->m_capacity, entry), true};
        }

        return {iterator(this->m_table, this->m_table + this->m_capacity, entry), false};
    }

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    template <typename... Args>
    pair<typename unordered_map<Key, T, Hash, KeyEqual, Allocator>::iterator, bool>
    unordered_map<Key, T, Hash, KeyEqual, Allocator>::m_try_emplace(const key_type& key, Args&&... args)
    {
        iterator it = this->find(key);

        if (it != this->end())
            return {it, false};

        return this->emplace(key, mapped_type(stl::forward<Args>(args)...));
    }

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    template <typename... Args>
    typename unordered_map<Key, T, Hash, KeyEqual, Allocator>::iterator
    unordered_map<Key, T, Hash, KeyEqual, Allocator>::m_try_emplace(const_iterator hint, const key_type& key, Args&&... args)
    {
        iterator it = this->find(key);

        if (it != this->end())
            return iterator(this->m_table, this->m_table + this->m_capacity, nullptr);

        return this->emplace_hint(hint, key, mapped_type(stl::forward<Args>(args)...));
    }
}