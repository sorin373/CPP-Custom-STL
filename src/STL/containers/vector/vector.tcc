namespace stl
{
    template <typename T, typename Alloc>
    vector<T, Alloc> &vector<T, Alloc>::operator=(const vector &other)
    {
        if (this != &other)
        {
            if (this->m_data != nullptr)
                m_alloc.deallocate(this->m_data, this->m_capacity);

            this->m_capacity = other.capacity();
            this->m_size = other.size();
            this->m_alloc = other.get_allocator();

            this->m_data = m_alloc.allocate(this->m_size);

            for (size_type i = 0; i < this->m_size; ++i)
                m_alloc.construct(this->m_data + i, other[i]);
        }

        return *this;
    }

    template <typename T, typename Alloc>
    vector<T, Alloc> &vector<T, Alloc>::operator=(const std::initializer_list<T> ilist)
    {
        if (this->m_data != nullptr)
        {
            for (size_type i = 0; i < this->m_size; ++i)
                this->m_alloc.destroy(this->m_data + i);

            this->m_alloc.deallocate(this->m_data, this->m_capacity);
        }

        this->m_capacity = this->m_size = ilist.size();

        this->m_data = this->m_alloc.allocate(this->m_size);

        const value_type *it = ilist.begin();

        for (size_type i = 0; i < this->m_size; ++i)
        {
            this->m_alloc.construct(m_data + i, *it);
            ++it;
        }
    }
}