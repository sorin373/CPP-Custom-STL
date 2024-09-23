namespace stl
{
    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    typename unordered_map<Key, T, Hash, KeyEqual, Allocator>::value_type* 
    unordered_map<Key, T, Hash, KeyEqual, Allocator>::m_get_table(const size_type bucket_count)
    {
        bucket_allocator alloc = this->m_alloc;
        value_type* __temp = alloc.allocate(bucket_count);
        return __temp; 
    }

    template <typename Key, typename T, typename Hash, typename KeyEqual, typename Allocator>
    void unordered_map<Key, T, Hash, KeyEqual, Allocator>::m_default_initialize(const size_type bucket_count)
    {
        this->m_table = this->get_m_table(bucket_count);

        for (size_type i = 0; i < bucket_count; ++i)
            *(this->m_table + i) = nullptr;
    }
}