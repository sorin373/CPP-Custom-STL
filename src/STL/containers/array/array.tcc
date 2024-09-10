#include "../../traits/type_traits.h"

namespace stl
{
    template <typename T, stl::size_t array_size>
    array<T, array_size>::array(const array& other)
    {
        if (other.size() > array_size) MEMORY_OVERFLOW_EXCEPTION

        for (size_type i = 0; i < array_size; ++i)
            this->m_data[i] = other.m_data[i];
    }

    template <typename T, stl::size_t array_size>
    array<T, array_size>::array(std::initializer_list<value_type> ilist)
    {
        if (ilist.size() > array_size) MEMORY_OVERFLOW_EXCEPTION

        const_iterator it = ilist.begin();

        for (size_type i = 0; i < array_size; ++i)
        {
            new(static_cast<void*>(this->m_data + i)) value_type(*it);
            ++it;
        }
    }

    template <typename T, stl::size_t array_size>
    array<T, array_size>& array<T, array_size>::operator=(const std::initializer_list<value_type>& ilist)
    {
        if (ilist.size() > array_size) MEMORY_OVERFLOW_EXCEPTION

        const_iterator it = ilist.begin();
        for (size_type i = 0; i < array_size; ++i)
        {
            new(static_cast<void*>(this->m_data + i)) value_type(*it);
            ++it;
        }
    }

    template <typename T, stl::size_t array_size>
    array<T, array_size>& array<T, array_size>::operator=(const array& other)
    {
        if (this != &other)
        {
            if (other.size() > array_size) MEMORY_OVERFLOW_EXCEPTION

            for (size_type i = 0; i < array_size; ++i)
                this->m_data[i] = other.m_data[i];
        }

        return *this;
    }

    template <typename T, stl::size_t array_size>
    typename array<T, array_size>::reference array<T, array_size>::at(size_type pos)
    {
        if (pos >= array_size) OUT_OF_BOUNDS_EXCEPTION
        return *(this->m_data + pos);
    }

    template <typename T, stl::size_t array_size>
    typename array<T, array_size>::const_reference array<T, array_size>::at(size_type pos) const
    {
        if (pos >= array_size) OUT_OF_BOUNDS_EXCEPTION
        return *(this->m_data + pos);
    }

    template <typename T, stl::size_t array_size>
    void array<T, array_size>::fill(const value_type value)
    {
        for (size_type i = 0; i < array_size; ++i)
            this->m_data[i] = value;
    }

    template <typename T, stl::size_t array_size>
    typename array<T, array_size>::reference array<T, array_size>::operator[](size_type pos)
    {
        if (pos >= array_size) OUT_OF_BOUNDS_EXCEPTION
        return *(this->m_data + pos);
    }

    template <typename T, stl::size_t array_size> constexpr typename array<T, array_size>::const_reference 
    array<T, array_size>::operator[](size_type pos) const
    {
        if (pos >= array_size) OUT_OF_BOUNDS_EXCEPTION
        return *(this->m_data + pos);
    }
}