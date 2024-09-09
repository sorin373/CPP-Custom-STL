#include "../../../mcmath.h"

namespace stl
{
    template <typename T, typename Allocator>
    vector<T, Allocator> &vector<T, Allocator>::operator=(const vector &other)
    {
        if (this != &other)
        {
            if (this->m_data != nullptr)
            {
                for (size_type i = 0; i < this->m_size; ++i)
                    this->m_alloc.destroy(this->m_data + i);

                this->m_alloc.deallocate(this->m_data, this->m_capacity);
            }
                

            this->m_capacity = other.capacity();
            this->m_size = other.size();
            this->m_alloc = other.get_allocator();

            this->m_data = m_alloc.allocate(this->m_size);

            for (size_type i = 0; i < this->m_size; ++i)
                m_alloc.construct(this->m_data + i, other[i]);
        }

        return *this;
    }

    template <typename T, typename Allocator>
    vector<T, Allocator> &vector<T, Allocator>::operator=(const std::initializer_list<T> ilist)
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

    template <typename T, typename Allocator> 
    void vector<T, Allocator>::assign(size_type count, const_reference value)
    {
        if (this->m_data != nullptr)
        {
            for (size_type i = 0; i < this->m_size; ++i)
                this->m_alloc.destroy(this->m_data + i);
            
            m_alloc.deallocate(this->m_data, this->m_capacity);
            this->m_size = this->m_capacity = 0;
        }

        if (count > 0)
        {
            this->m_size = this->m_capacity = count;
            this->m_data = this->m_alloc.allocate(count);
            
            try 
            {
                for (size_type i = 0; i < count; ++i)
                    this->m_alloc.construct(this->m_data + i, value);
            }
            catch(...)
            {
                this->alloc_fallback();
                throw;
            }
        }
    }

    template <typename T, typename Allocator>
    template <typename InputIt, typename>
    void vector<T, Allocator>::assign(InputIt first, InputIt last)
    {
        if (first == nullptr || last == nullptr)
            return;

        if (this->m_data != nullptr)
        {
            for (size_type i = 0; i < this->m_size; ++i)
                this->m_alloc.destroy(this->m_data + i);
            
            m_alloc.deallocate(this->m_data, this->m_capacity);
            this->m_size = this->m_capacity = 0;
        }

        difference_type size = stl::distance(first, last);

        this->m_size = this->m_capacity = size;
        this->m_data = this->m_alloc.allocate(size);
        
        try
        {
            for (size_type i = 0; i < size; ++i)
            {   
                this->m_alloc.construct(this->m_data + i, *first);
                ++first;
            }   
        }
        catch(...)
        {
            this->alloc_fallback();
            throw;
        }
    }

    template <typename T, typename Allocator>
    void vector<T, Allocator>::assign(std::initializer_list<value_type> ilist)
    {
        if (this->m_data != nullptr)
        {
            for (size_type i = 0; i < this->m_size; ++i)
                this->m_alloc.destroy(this->m_data + i);

            m_alloc.deallocate(this->m_data, this->m_capacity);
            this->m_size = this->m_capacity = 0;
        }

        this->m_size = this->m_capacity = ilist.size();
        this->m_data = this->m_alloc.allocate(this->m_size);

        try
        {
            const_iterator it = ilist.begin();
            for (size_type i = 0; i < this->m_size; ++i)
            {
                this->m_alloc.construct(this->m_data + i, *it);
                ++it;
            }
        }
        catch(...)
        {
            this->alloc_fallback();
            throw;
        }
    }

    template <typename T, typename Allocator>
    void vector<T, Allocator>::reserve(size_type new_cap)
    {
        if (new_cap > this->max_size())
            throw std::length_error("new_cap > max_size()");

        if (new_cap <= this->m_capacity)
            return;

        value_type *temp = this->m_alloc.allocate(new_cap);

        this->m_alloc.deallocate(this->m_data, this->m_capacity);
        this->m_capacity = new_cap;
        this->m_data = temp;
    }

    template <typename T, typename Allocator>
    void vector<T, Allocator>::shrink_to_fit()
    {
        if (this->m_capacity == this->m_size)
            return;

        value_type *temp = this->m_alloc.allocate(this->m_size);
    
        memcpy(temp, this->m_data, this->m_size * sizeof(value_type));

        this->m_alloc.deallocate(this->m_data, this->m_capacity);
        this->m_capacity = this->m_size;
        this->m_data = temp;
    }

    template <typename T, typename Allocator>
    void vector<T, Allocator>::clear() noexcept
    {
        for (size_type i = 0; i < this->m_size; ++i)
            this->m_alloc.destroy(this->m_data + i);

        this->m_size = 0;
    }

    template <typename T, typename Allocator>
    typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos, const_reference value)
    {
        difference_type index = stl::distance(cbegin(), pos);

        if (index > this->m_size) OUT_OF_BOUNDS_EXCEPTION

        ++this->m_size;

        if (this->m_size > this->m_capacity)
            this->resize(this->m_size);

        for (int i = this->m_size - 1; i > index; --i)
            this->m_data[i] = this->m_data[i - 1];

        this->m_alloc.destroy(this->m_data + index);
        this->m_alloc.construct(this->m_data + index, value);

        return iterator(this->m_data + index);
    }

    template <typename T, typename Allocator>
    typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos, size_type count, const_reference value)
    {
        if (count == 0)
            return iterator(pos);

        difference_type index = stl::distance(cbegin(), pos);

        if (index > this->m_size) OUT_OF_BOUNDS_EXCEPTION

        this->m_size += count;

        if (this->m_size > this->m_capacity)
            this->resize(this->m_size);

        // move all items `size` spaces to the right
        for (int i = this->m_size - count; i > index; --i)
            this->m_data[i + count - 1] = this->m_data[i - 1];
            
        for (int i = index, N = index + count; i < N; ++i)
        {
            this->m_alloc.destroy(this->m_data + i);
            this->m_alloc.construct(this->m_data + i, value);
        }

        return iterator(this->m_data + index);
    }

    template <typename T, typename Allocator>
    template <typename InputIt, typename>
    typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos, InputIt first, InputIt last)
    {
        if (first == last)
            return iterator(pos);

        difference_type index = stl::distance(cbegin(), pos);

        if (index > this->m_size) OUT_OF_BOUNDS_EXCEPTION

        difference_type add_size = stl::distance(first, last);

        this->m_size += add_size;

        if (this->m_size > this->m_capacity)
            this->resize(this->m_size);

        for (int i = this->m_size - add_size; i >= index; --i)
            this->m_data[i + add_size - 1] = this->m_data[i - 1];

        for (int i = index, N = index + add_size; i < N; ++i)
        {
            this->m_alloc.destroy(this->m_data + i);
            this->m_alloc.construct(this->m_data + i, *first);

            ++first;
        }

        return iterator(this->m_data + index);
    }

    template <typename T, typename Allocator>
    typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos, std::initializer_list<value_type> ilist)
    {
        int add_size = ilist.size();
        
        if (add_size == 0)
            return iterator(pos);

        difference_type index = stl::distance(cbegin(), pos);

        if (index > this->m_size) OUT_OF_BOUNDS_EXCEPTION

        this->m_size += add_size;

        if (this->m_size > this->m_capacity)
            this->resize(this->m_size);

        for (int i = this->m_size - add_size; i >= index; --i)
            this->m_data[i + add_size - 1] = this->m_data[i - 1];

        const_iterator it = ilist.begin();
        for (int i = index, N = index + add_size; i < N; ++i)
        {
            this->m_alloc.destroy(this->m_data + i);
            this->m_alloc.construct(this->m_data + i, *it);

            ++it;
        }

        return iterator(this->m_data + index);
    }

    template <typename T, typename Allocator>
    void vector<T, Allocator>::resize(size_type count)
    {
        if (count < this->m_size)
        {
            for (size_type i = count; i < this->m_size; ++i)
                this->m_alloc.destroy(this->m_data + i);

            this->m_size = count;
            
            return;
        }
        else if (count <= this->m_capacity)
            return;

        size_type new_capacity = count + count / 2 + 1;

        value_type *temp = this->m_alloc.allocate(new_capacity);

        stl::memcpy(temp, this->m_data, this->m_size * sizeof(value_type));

        this->m_alloc.deallocate(this->m_data, this->m_capacity);
        
        this->m_size = count;
        this->m_capacity = new_capacity;
        this->m_data = temp;
    }

    template <typename T, typename Allocator>
    void vector<T, Allocator>::resize(size_type count, const_reference value)
    {
        if (count < this->m_size)
        {
            for (size_type i = count; i < this->m_size; ++i)
                this->m_alloc.destroy(this->m_data + i);

            this->m_size = count;
            
            return;
        }
        else if (count <= this->m_capacity)
            return;

        size_type new_capacity = count + count / 2 + 1;

        value_type *temp = this->m_alloc.allocate(new_capacity);
        
        stl::memcpy(temp, this->m_data, this->m_size * sizeof(value_type));

        for (size_type i = this->m_size; i < count; ++i)
            this->m_alloc.construct(temp + i, value);

        this->m_capacity = new_capacity;
        this->m_size = count;
        this->m_data = temp;
    }

    template <typename T, typename Allocator>
    void vector<T, Allocator>::push_back(const_reference element)
    {
        if (this->m_size + 1 > this->m_capacity)
            this->resize(this->m_size + 1);
        else
            ++this->m_size;

        this->m_alloc.construct(this->m_data + this->m_size - 1, element);
    }
    
    template <typename T, typename Allocator>
    void vector<T, Allocator>::pop_back()
    {
        if (this->m_size > 0)
        {
            this->m_alloc.destroy(this->m_data + this->m_size - 1);
            --this->m_size;
        }
    }
}