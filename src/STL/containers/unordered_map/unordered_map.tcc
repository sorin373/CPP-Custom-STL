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

    /// @b private_members

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
    unordered_map<Key, T, Hash, KeyEqual, Allocator>::m_get_node(const key_type& key, const mapped_type& value)
    {
        pointer __new_node = this->m_alloc.allocate(1);

        new (&__new_node->m_pair) stl::pair<const key_type, mapped_type>(key, value);
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
}