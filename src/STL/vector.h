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

    @file vector.h

    @brief This is part of the standard template libraries (STL) and is used to implement resizeable arrays
           and commonly used algorithms.
 */

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "../memory/memory.h"
#include "reverse_iterator.h"

#include <initializer_list>
#include <stdlib.h>
#include <math.h>

#define INITIAL_VECTOR_SIZE 0
#define VECTOR_BYTE_SIZE(__l) (__l) * sizeof(T)                                    // Calculates the memory size needed to allocate for a vector of a certain length __l
#define OUT_OF_BOUNDS_EXCEPTION throw std::out_of_range("Index out of bounds!\n"); // Exception macro

namespace stl
{
    /**
     *  @brief Vector (STL) container. It adds extra functionalites to the basic CPP arrays, by resizing the memory when necessary.
     *         In addition it consists in many member functions that help the user manage the array data faster and more efficiently.
     *  @param T Type of data 
     */
    template <typename T> class vector
    {
        typedef size_t  size_type;
        typedef T       value_type;

        // Private aliases for the reverse iterators. The user will use the generic reverse_iterator template.
        typedef reverse_iterator<value_type>       reverse_iterator;
        typedef const_reverse_iterator<value_type> const_reverse_iterator;

        typedef vector& vector_reference;

    public:
        /**
         * @brief Define type aliases for the iterators types.
         *        These iterators provide random access capabilities and can be used to traverse containers that store elements of value_type
         * 
         * @example 
         * for (iterator it = vector.begin(); it != vector.end(); it++) {
         *   Access each element using *it
         * }
         */
        typedef T* iterator;
        typedef T& reference;

        /**
         * @brief Define type aliases for the references types.
         *        They provide a way to refer to elements within the container and can be used to access or modify the element.
         */
        typedef const T* const_iterator;
        typedef const T& const_reference;

        vector() noexcept : m_capacity(0), m_size(0), m_data(nullptr) { }

        /**
         * @throw If the new memory block can not be allocated, a runtime error will be thrown.
         */
        vector(std::initializer_list<value_type> init) : m_capacity(init.size()), m_size(init.size()), m_data(nullptr)
        {
            this->m_data = (T *)malloc(VECTOR_BYTE_SIZE(m_size));

            if (this->m_data == nullptr)
                throw std::runtime_error("malloc: couldn't allocate memory!\n");

            memcpy(m_data, init.begin(), VECTOR_BYTE_SIZE(m_size));
        }

        /**
         * @returns The number of elements currently present in the vector.
         */
        size_type size() const noexcept { return m_size; }

        /**
         * @brief (2 ^ nativePointerBitWidth) / sizeof(dataType) - 1
         * @returns The theoretical maximum number of items that could be put in the vector.
         */
        constexpr uint64_t max_size() { return pow(2, ENV) / sizeof(value_type) - 1; }
        
        size_type capcacity() const noexcept { return m_capacity; }

        /**
         * @returns Returns true if the vector has no elements.
         */
        bool empty() const noexcept { return m_size == INITIAL_VECTOR_SIZE; }

        /**
         * @brief This function resizes the container. While the m_size increses by 1 the m_capacity is calculated 
         *        to be >= to m_size so that the container is resized efficently. The formula I came up with is:
         *        m_capacity = new_size + new_size / 2 + 1. It is designed to grow as the new_size gets bigger and bigger,
         *        balancing the need to minimize the number of resizes and the memory overhead.
         * @param new_size The new size to which the vector will be resized to
         * @throw If the new memory block can not be allocated, a runtime error will be thrown.
         */
        void resize(size_type new_size)
        {
            if (new_size < m_size)
            {
                m_size = new_size;
                return;
            }
                
            m_capacity = new_size + new_size / 2 + 1;

            T *temp_ptr = (T *)malloc(VECTOR_BYTE_SIZE(m_capacity));

            if (temp_ptr == nullptr)
                throw std::runtime_error("malloc: couldn't allocate memory!\n");

            memcpy(temp_ptr, m_data, VECTOR_BYTE_SIZE(m_size - 1));

            free(m_data);

            m_data = temp_ptr;
        }

        /**
         * @brief This function frees any memory which is not used by the vector. In other words, the m_capacity will equal to m_size.
         * @attention The function will return if the m_capacity is already equal to m_size.
         * @throw If the new memory block can not be allocated, a runtime error will be thrown.
         */
        void shrink_to_fit()
        {
            if (m_capacity == m_size)
                return;

            T *temp_ptr = (T *)malloc(VECTOR_BYTE_SIZE(m_size));

            if (temp_ptr == nullptr)
                throw std::runtime_error("malloc: couldn't allocate memory!\n");

            m_capacity = m_size;

            memcpy(temp_ptr, m_data, VECTOR_BYTE_SIZE(m_capacity));

            free(m_data);

            m_data = temp_ptr;
        }

        /**
         * @brief This function reserves a chunk of memory for the vector container.
         * @param size The new size to which the vector will be resized to
         * @throw If the new memory block can not be allocated, a runtime error will be thrown.
         */
        void reserve(size_type size)
        {
            if (size <= m_capacity)
                return;

            T *temp_ptr = (T *)malloc(VECTOR_BYTE_SIZE(size));

            if (temp_ptr == nullptr)
                throw std::runtime_error("malloc: couldn't allocate memory!\n");

            m_capacity = size;

            memcpy(temp_ptr, m_data, VECTOR_BYTE_SIZE(m_size));

            free(m_data);

            m_data = temp_ptr;
        }

        void push_back(value_type element)
        {
            m_size++;

            if (m_size > m_capacity)
                resize(m_size);

            m_data[m_size - 1] = element;
        }

        void pop_back()
        {
            if (m_size > 0)
            {
                m_data[m_size - 1].~T();

                m_size--;
            }
        }

        void insert(const_iterator position, value_type value)
        {
            int index = get_index(position, this->begin());

            if (index >= m_size) OUT_OF_BOUNDS_EXCEPTION

            m_size++;

            if (m_size > m_capacity)
                resize(m_size);

            for (size_type i = this->size() - 1; i >= index; i--)
                m_data[i + 1] = m_data[i];

            m_data[index] = value;
        }

        void insert(const_iterator position, size_type size, value_type value)
        {
            size_type index = get_index(position, this->begin());

            if (index >= m_size) OUT_OF_BOUNDS_EXCEPTION

            size_type old_size = m_size;                      // use 'old_size' var to get the index of the last item in the vector before the insert

            m_size += size;

            if (m_size > m_capacity)
                resize(m_size);

            for (size_type i = old_size - 1; i >= index; i--) // move all items 'size' spaces to the right
                m_data[i + size] = m_data[i];

            for (size_type i = index; i <= index + size - 1; i++)
                m_data[i] = value;
        }

        void insert(const_iterator position, iterator it_1, iterator it_2)
        {
            int index = get_index(position, this->begin());

            if (index >= m_size) OUT_OF_BOUNDS_EXCEPTION

            int additional_size = std::abs(it_1 - it_2);

            size_type old_size = m_size;

            m_size += additional_size;

            if (m_size > m_capacity)
                resize(m_size);

            for (size_type i = old_size - 1; i >= index; i--)
                m_data[i + additional_size] = m_data[i];

            for (size_type i = index; i <= index + additional_size - 1; i++)
                m_data[i] = *it_1++;
        }

        void clear() noexcept
        { 
            for (size_type i = 0, n = this->size(); i < n; i++) m_data[i].~T();

            if (m_size > 0) m_size = INITIAL_VECTOR_SIZE;
        }

        void erase(iterator position)
        {
            if (position >= this->end()) OUT_OF_BOUNDS_EXCEPTION

            for (iterator it = position; it != this->end() - 1; it++)
                *it = *(it + 1);

            m_size--;
        }

        void erase(iterator begin, iterator end)
        {
            if (begin >= this->end() || end >= this->end()) OUT_OF_BOUNDS_EXCEPTION

            if (begin == end)
            {
                this->erase(begin);
                return;
            }

            size_type size_deleted = std::abs(begin - end);

            m_size -= size_deleted;

            if (begin > end)
            {
                iterator temp;

                temp  = begin;
                begin = end;
                end   = temp;
            }

            iterator it_original = end;

            for (iterator it = begin; it != end && it_original != this->end(); it++)
                *it = *it_original++;
        } 

        void swap(vector &payload) noexcept
        {
            iterator  temp_m_data     = this->m_data;
            size_type temp_m_size     = this->m_size;
            size_type temp_m_capacity = this->m_capacity;

            this->m_data              = payload.data();
            this->m_size              = payload.size();
            this->m_capacity          = payload.capcacity();

            payload.m_data            = temp_m_data;
            payload.m_size            = temp_m_size;
            payload.m_capacity        = temp_m_capacity;
        }

        template <typename... Args> iterator emplace(iterator position, Args... args)
        {
            int index = get_index(this->begin(), position); // int type bec for some reason the for breaks if unsigned int 
            size_type old_size = m_size;

            if (index >= m_size) OUT_OF_BOUNDS_EXCEPTION

            m_size++;

            if (m_size > m_capacity)
                resize(m_size);

            for (int i = old_size - 1; i >= index; i--)
                m_data[i + 1] = m_data[i];

            new (&m_data[index]) value_type(std::forward<Args>(args)...); // new (ptr) T(args) - placement new. It constructs an object of type T at the address ptr without allocating new memory

            return &m_data[index];
        }

        template <typename... Args> iterator emplace_back(Args... args)
        {
            m_size++;

            if (m_size > m_capacity)
                resize(m_size);

            new (&m_data[m_size - 1]) value_type(std::forward<Args>(args)...);

            return &m_data[m_size - 1];
        }

        iterator find(value_type value)
        {
            for (iterator it = this->begin(); it != this->end(); it++)
                if (*it == value)
                    return it;

            return nullptr;
        }

        reference at(size_type index) 
        { 
            if (index >= m_size) OUT_OF_BOUNDS_EXCEPTION

            return m_data[index];
        }

        reference front() noexcept { return m_data[0]; }

        reference back() noexcept  { return m_data[m_size - 1]; }

        iterator data() noexcept   { return m_data; }

        void assign(size_type size, const value_type value)
        {
            if (m_size > INITIAL_VECTOR_SIZE)
                return;

            m_size = m_capacity = size;

            m_data = (value_type *)malloc(VECTOR_BYTE_SIZE(m_size));

            if (m_data == nullptr)
                throw std::runtime_error("malloc: couldn't allocate memory!\n");

            for (iterator it = begin(); it != end(); it++)
                *it = value;
        }

        void assign(iterator first, iterator last)
        {
            if (first == nullptr || last == nullptr)
                return;

            int size = first - last;

            if (size < 0) size = -size;

            m_data = (value_type *)malloc(VECTOR_BYTE_SIZE(size));

            if (m_data == nullptr)
                throw std::runtime_error("malloc: couldn't allocate memory!\n");

            memcpy(m_data, first, VECTOR_BYTE_SIZE(size));

            m_size = m_capacity = size;
        }

        void assign(std::initializer_list<value_type> init)
        { 
            m_size = m_capacity = init.size();

            m_data = (T *)malloc(VECTOR_BYTE_SIZE(m_size));

            if (m_data == nullptr)
                throw std::runtime_error("malloc: couldn't allocate memory!\n");

            memcpy(m_data, init.begin(), VECTOR_BYTE_SIZE(m_size));
        }

        iterator begin()                       { return m_data; }

        const_iterator cbegin() const noexcept { return m_data; }

        iterator end() noexcept                { return m_data + m_size; }

        const_iterator cend() const noexcept   { return m_data + m_size; }

        reverse_iterator rbegin()              { return reverse_iterator(m_data + m_size); }

        reverse_iterator rend()                { return reverse_iterator(m_data); }

        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(m_data + m_size); }

        const_reverse_iterator crend() const noexcept   { return const_reverse_iterator(m_data); } 

        reference operator[](size_type index) 
        { 
            if (index >= m_size) OUT_OF_BOUNDS_EXCEPTION

            return m_data[index];
        }

        const_reference operator[](size_type index) const 
        {
            if (index >= m_size) OUT_OF_BOUNDS_EXCEPTION

            return m_data[index];
        }

        vector_reference operator=(const vector other) 
        { 
            if (this != &other)
            {
                if (m_data != nullptr)
                    free(m_data);

                m_capacity = other.capcacity();
                m_size     = other.size();

                m_data = (T *)malloc(VECTOR_BYTE_SIZE(m_size));

                if (m_data == nullptr)
                    throw std::runtime_error("malloc: couldn't allocate memory!\n");

                memcpy(this->m_data, other.cbegin(), VECTOR_BYTE_SIZE(m_size));
            }

            return *this;
        }

        ~vector() { free(m_data); }

    private:
        value_type *m_data;
        size_type   m_size;
        size_type   m_capacity;

        constexpr size_type get_index(iterator it_1, iterator it_2) const noexcept { return std::abs(it_1 - it_2); }
    };
}

#endif // VECTOR_H