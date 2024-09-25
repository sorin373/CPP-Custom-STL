namespace stl
{
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

    /// @c private_members

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    typename unordered_map<Key, T, Hash, KeyEqual, Allocator>::pointer*
    unordered_map<Key, T, Hash, KeyEqual, Allocator>::m_get_table(const size_type bucket_count)
    {
        bucket_allocator __alloc = this->m_alloc;
        value_type** __temp = __alloc.allocate(bucket_count);
        return __temp; 
    }

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    typename unordered_map<Key, T, Hash, KeyEqual, Allocator>::pointer
    unordered_map<Key, T, Hash, KeyEqual, Allocator>::m_get_node(key_type&& key, mapped_type&& value)
    {
        pointer __new_node = this->m_alloc.allocate(1);

        new (&__new_node->m_pair) stl::pair<key_type, mapped_type>(stl::forward<key_type>(key), stl::forward<mapped_type>(value));
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
            this->m_capacity *= 2;
            this->rehash(this->m_capacity);
        }
    }

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    template <typename... Args>
    pair_node<typename unordered_map<Key, T, Hash, KeyEqual, Allocator>::iterator, bool>
    unordered_map<Key, T, Hash, KeyEqual, Allocator>::m_insert(Args&&... args)
    {
        this->m_check_rehash(this->m_size, this->m_capacity, this->m_load_factor);

        stl::pair_node<key_type, mapped_type> node(stl::forward<Args>(args)...);

        key_type key = node.m_pair.first;
        mapped_type value = node.m_pair.second;

        size_type hash_value = this->hash(key);
        pointer prev = nullptr, entry = *(this->m_table + hash_value);

        while (entry != nullptr && !this->m_key_equal(entry->m_pair.first, key))
        {
            prev = entry;
            entry = entry->m_next;
        }

        if (entry == nullptr)
        {
            entry = this->m_get_node(stl::move(key), stl::move(value));
            
            if (prev == nullptr)
                this->m_table[hash_value] = entry;
            else
                prev->m_next = entry;

            ++this->m_size;

            return {iterator(this->m_table, this->m_table + this->m_capacity, entry), true};
        }
        else
            entry->m_pair.second = value;

        return {iterator(this->m_table, this->m_table + this->m_capacity, entry), false};
    }

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    template <typename... Args>
    typename unordered_map<Key, T, Hash, KeyEqual, Allocator>::iterator
    unordered_map<Key, T, Hash, KeyEqual, Allocator>::m_insert(const_iterator hint, Args&&... args)
    {
        this->m_check_rehash(this->m_size, this->m_capacity, this->m_load_factor);

        stl::pair_node<key_type, mapped_type> node(stl::forward<Args>(args)...);

        key_type key = node.m_pair.first;
        mapped_type value = node.m_pair.second;

        size_type hash_value = this->hash(key);
        pointer prev = nullptr, entry = *(this->m_table + hash_value);

        while (entry != nullptr && !this->m_key_equal(entry->m_pair.first, key))
        {
            prev = entry;
            entry = entry->m_next;
        }

        if (entry == nullptr)
        {
            entry = this->m_get_node(stl::move(key), stl::move(value));

            if (prev == nullptr)
                this->m_table[hash_value] = entry;
            else
                prev->m_next = entry;

            ++this->m_size;

            return iterator(this->m_table, this->m_table + this->m_capacity, entry);
        }
        else
            entry->m_pair.second = value;

        return iterator(this->m_table, this->m_table + this->m_capacity, entry);
    }
}