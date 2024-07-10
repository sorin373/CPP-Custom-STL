#ifndef __ARRAY_H__
#define __ARRAY_H__ 1

#include "memory/memory.h"

#define OUT_OF_BOUNDS_EXCEPTION throw std::out_of_range("Index out of bounds!\n");

namespace stl
{
    template <typename T, size_t array_size> class array
    {
        typedef T      value_type;
        typedef size_t size_type;

    public:
        typedef value_type* iterator;
        typedef value_type& reference;

        typedef const iterator  const_iterator;
        typedef const reference const_reference;

        array() : m_data(nullptr) 
        {
            m_data = (value_type *)malloc(array_size * sizeof(value_type));

            if (m_data == nullptr)
                throw std::runtime_error("malloc: couldn't allocate memory!\n");
        }

        value_type at(size_type index) 
        { 
            if (index >= array_size) OUT_OF_BOUNDS_EXCEPTION

            return m_data[index]; 
        }

        reference operator[](size_type index)
        {
            if (index >= array_size) OUT_OF_BOUNDS_EXCEPTION

            return m_data[index];
        }

        const_reference operator[](size_type index) const
        {
            if (index >= array_size) OUT_OF_BOUNDS_EXCEPTION

            return m_data[index];
        }

    private:
        value_type *m_data;
    };
}

#endif