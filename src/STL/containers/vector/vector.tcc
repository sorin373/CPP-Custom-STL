#include "../../../cUtility/move.h"
#include "../../traits/type_traits.h"

#include <cstring>

namespace stl
{
    template <typename T, typename Allocator> 
    void vector<T, Allocator>::assign(size_type count, const_reference value)
    {
        if (this->m_data != nullptr)
        {
            if (count > this->m_capacity)
                try 
                { 
                    this->reserve(count); 
                }
                catch(...)
                {
                    this->alloc_fallback();
                    throw;
                }

            this->clear();
        }
        else if (count > 0)
        {
            this->m_data = this->m_alloc.allocate(count);
            this->m_capacity = count;
        }

        if (count > 0)
        {
            this->m_size = count;
            this->m_default_initialize(count, value);
        }
    }

    template <typename T, typename Allocator>
    template <typename InputIt, typename>
    void vector<T, Allocator>::assign(InputIt first, InputIt last)
    {
        if (first == last) 
            return;

        difference_type size = stl::distance(first, last);

        if (this->m_data != nullptr)
        {
            if (size > this->m_capacity)
                try 
                { 
                    this->reserve(size); 
                }
                catch(...)
                {
                    this->alloc_fallback();
                    throw;
                }

            this->clear();
        }
        else
        {
            this->m_data = this->m_alloc.allocate(size);
            this->m_capacity = size;
        }

        try
        {
            for (size_type i = 0; first != last; ++i, ++first)
                this->m_alloc.construct(this->m_data + i, *first);
        }
        catch(...)
        {
            this->alloc_fallback();
            throw;
        }

        this->m_size = size;
    }

    template <typename T, typename Allocator>
    void vector<T, Allocator>::reserve(size_type new_cap)
    {
        if (new_cap <= this->m_capacity)
            return;

        value_type *temp = this->m_alloc.allocate(new_cap);

        if (stl::is_trivially_copyable<value_type>::value)
            std::memcpy(temp, this->m_data, this->m_size * sizeof(value_type));
        else
        {
            for (size_type i = 0; i < this->m_size; ++i)
            {
                this->m_alloc.construct(temp + i, stl::move(*(this->m_data + i)));
                this->m_alloc.destroy(this->m_data + i);
            }
        }

        if (this->m_data)
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

        for (size_type i = 0; i < this->m_size; ++i)
            this->m_alloc.construct(temp + i, *(this->m_data + i));

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
    typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator pos, size_type count, const_reference value)
    {
        if (count == 0)
            return iterator(pos);

        difference_type index = stl::distance(this->cbegin(), pos);

        if (index > this->m_size) OUT_OF_BOUNDS_EXCEPTION

        if (this->m_size + count > this->m_capacity)
            this->resize(this->m_size + count);

        for (int i = this->m_size - count - 1; i >= index; --i)
            this->m_data[i + count] = stl::move(this->m_data[i]);
            
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

        difference_type index = stl::distance(this->cbegin(), pos);

        if (index > this->m_size) OUT_OF_BOUNDS_EXCEPTION

        difference_type add_size = stl::distance(first, last);

        if (this->m_size + add_size > this->m_capacity)
            this->resize(this->m_size + add_size);

        for (int i = this->m_size - add_size - 1; i >= index; --i)
            this->m_data[i + add_size] = stl::move(this->m_data[i]);

        for (int i = index; first != last; ++i, ++first)
        {
            this->m_alloc.destroy(this->m_data + i);
            this->m_alloc.construct(this->m_data + i, *first);
        }

        return iterator(this->m_data + index);
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

        if (count <= this->m_capacity)
        {
            for (size_type i = this->m_size; i < count; ++i)
                this->m_alloc.construct(this->m_data + i, value);
            
            this->m_size = count;
            return;
        }

        size_type new_capacity = count + count / 2 + 1;
        value_type *temp = this->m_alloc.allocate(new_capacity);

        for (size_type i = 0; i < this->m_size; ++i)
        {
            this->m_alloc.construct(temp + i, stl::move(this->m_data[i]));
            this->m_alloc.destroy(this->m_data + i);
        }

        for (size_type i = this->m_size; i < count; ++i)
            this->m_alloc.construct(temp + i, value);
        
        if (this->m_data)
            this->m_alloc.deallocate(this->m_data, this->m_capacity);

        this->m_data = temp;
        this->m_size = count;
        this->m_capacity = new_capacity;
    }

    template <typename T, typename Allocator>
    void vector<T, Allocator>::push_back(const_reference element)
    {
        if (this->m_size == this->m_capacity)
            this->reserve(this->m_capacity * 2 + 1);
                
        this->m_alloc.construct(this->m_data + this->m_size, element);
        ++this->m_size;
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

    template <typename T, typename Allocator>
    void vector<T, Allocator>::erase(iterator begin, iterator end)
    {
        iterator _end = this->end();

        if (begin > _end || end > _end) OUT_OF_BOUNDS_EXCEPTION

        if (begin == end)
            return;

        size_type deleted_size = stl::distance(begin, end);

        for (iterator it = begin, it_org = end; it_org != _end; ++it, ++it_org)
            *it = *it_org;

        for (iterator it = _end - deleted_size; it != _end; ++it)
            this->m_alloc.destroy(it);

        this->m_size -= deleted_size;
    }

    template <typename T, typename Allocator>
    void vector<T, Allocator>::swap(vector& other) noexcept
    {
        stl::swap(this->m_data,     other.m_data);
        stl::swap(this->m_alloc,    other.m_alloc);
        stl::swap(this->m_size,     other.m_size);
        stl::swap(this->m_capacity, other.m_capacity);
    }
    
    template <typename T, typename Allocator>
    template <typename... Args>
    typename vector<T, Allocator>::iterator
    vector<T, Allocator>::emplace(iterator pos, Args&&... args)
    {
        difference_type index = stl::distance(this->begin(), pos);

        if (index < 0 || index > (difference_type)this->m_size) 
            OUT_OF_BOUNDS_EXCEPTION;

        if (this->m_size == this->m_capacity)
            this->reserve(this->m_capacity * 2 + 1);

        iterator itpos = this->begin() + index;

        if ((size_type)index == this->m_size)
        {
            this->m_alloc.construct(this->m_data + this->m_size, stl::forward<Args>(args)...);
            ++this->m_size;
            
            return this->begin() + (this->m_size - 1);
        }

        this->m_alloc.construct(this->m_data + this->m_size, stl::move(this->m_data[this->m_size - 1]));

        for (size_type i = this->m_size - 1; i > (size_type)index; --i)
            this->m_data[i] = stl::move(this->m_data[i - 1]);

        this->m_alloc.destroy(this->m_data + (size_type)index);
        this->m_alloc.construct(this->m_data + (size_type)index, stl::forward<Args>(args)...);

        ++this->m_size;
        return this->begin() + index;
    }

    template <typename T, typename Allocator>
    template <typename... Args>
    typename vector<T, Allocator>::iterator vector<T, Allocator>::emplace_back(Args&&... args)
    {
        if (this->m_size == this->m_capacity)
            this->reserve(this->m_capacity * 2 + 1);

        this->m_alloc.construct(this->m_data + this->m_size, stl::forward<Args>(args)...);

        ++this->m_size;

        return iterator(this->m_data + this->m_size - 1);
    }

    template <typename T, typename Allocator>
    typename vector<T, Allocator>::iterator vector<T, Allocator>::find(value_type value)
    {
        for (size_type i = 0; i < this->m_size; ++i)
        {
            iterator it = this->m_data + i;

            if (*it == value)
                return it; 
        }

        return nullptr;
    }

    template <typename T, typename Allocator>
    void vector<T, Allocator>::alloc_fallback()
    {
        for (size_type i = 0; i < this->m_size; ++i)
            this->m_alloc.destroy(this->m_data + i);
                
        this->m_alloc.deallocate(this->m_data, this->m_size);
        this->m_size = this->m_capacity = 0;
        this->m_data = nullptr;
    }

    template <typename T, typename Allocator>
    void vector<T, Allocator>::m_default_initialize(size_type count, const_reference value)
    {
        if (count > 0)
        {
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
    template <typename InputIt>
    void vector<T, Allocator>::m_range_initialize(InputIt first, InputIt last)
    {
        if (this->m_size > 0)
        {
            this->m_data = this->m_alloc.allocate(this->m_size);
                        
            try
            {
                for (size_type i = 0; first != last; ++first, ++i)
                    this->m_alloc.construct(this->m_data + i, *first);
            }
            catch(...)
            {
                this->alloc_fallback();
                throw;
            }
        }
    }
}