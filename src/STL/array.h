/**
 *  @copyright MIT License

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

    @file array.h

    @brief This is part of the standard template libraries (STL) and it is used to implement fixed size arrays
           and commonly used algorithms.
 */

#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "core.h"
#include "memory.h"

#include <initializer_list>
#include <stdexcept>

#include <iterator> // stl iterator not working so std::reverse_iterator is used for now

#define OUT_OF_BOUNDS_EXCEPTION   throw std::out_of_range("Index out of bounds!\n");
#define MEMORY_OVERFLOW_EXCEPTION throw std::runtime_error("Too many elements. Check the array size to be correct!\n");

#define META_SIZE sizeof(T)

namespace stl
{
    /**
     * @brief This is a container that encapsulates fixed size arrays.
     * 
     * @param T          Type of element
     * @param array_size The fixed no. of elements that the array can have
     */
    template <
        typename T, 
        stl::size_t array_size> 
    class array
    {
        constexpr static int __SIZE__ = array_size * META_SIZE;

    public:
        typedef T                    value_type;
        typedef stl::size_t          size_type;
        typedef stl::ptrdiff_t       difference_type;

        typedef T&                   reference;
        typedef const T&             const_reference;

        typedef       value_type*    pointer;
        typedef const value_type*    const_pointer;

        typedef value_type*          iterator;
        typedef const value_type*    const_iterator;

        typedef std::reverse_iterator<iterator>         reverse_iterator;
        typedef std::reverse_iterator<iterator>   const_reverse_iterator;

        array() noexcept { }

        array(const array& other)
        {
            if (other.size() > array_size) MEMORY_OVERFLOW_EXCEPTION

            for (size_type i = 0; i < array_size; ++i)
                m_data[i] = other.m_data[i];
        }

        array(std::initializer_list<value_type> init)
        {
            if (init.size() > array_size) MEMORY_OVERFLOW_EXCEPTION

            const T* it = init.begin();
            for (size_type i = 0; i < array_size; ++i)
            {
                new(static_cast<void*>(m_data + i)) T(*it);
                ++it;
            }
        }
        
        array& operator=(const std::initializer_list<value_type>& init)
        {
            if (init.size() > array_size) MEMORY_OVERFLOW_EXCEPTION

            const T* it = init.begin();
            for (size_type i = 0; i < array_size; ++i)
            {
                new(static_cast<void*>(m_data + i)) T(*it);
                ++it;
            }
        }
        
        /** 
         * @brief Overwrites every element of the array with the corresponding element of another array 
         * @param other array whose contents are used to override 'this'
        */
        array& operator=(const array& other)
        {
            if (this != &other)
            {
                if (other.size() > array_size) MEMORY_OVERFLOW_EXCEPTION

                for (size_type i = 0; i < array_size; ++i)
                    m_data[i] = other[i];
            }

            return *this;
        }

        constexpr size_type max_size() const noexcept { return array_size; }

        constexpr size_type size() const noexcept     { return array_size; }

        constexpr bool empty() const noexcept { return array_size == 0; }

        pointer data() noexcept { return m_data; } 

        const_pointer data() const noexcept { return m_data; }  

        /**
         * @brief Returns a reference to an element within the container
         * @param index The location of the element
         * @return a reference to the requested element
         * @throws std::out_of_range if the index is out of bounds
         */
        reference at(size_type index) 
        { 
            if (index >= array_size) OUT_OF_BOUNDS_EXCEPTION

            return m_data[index]; 
        }

        /**
         * @brief Returns a constant reference to an element within the container
         * @param index The location of the element
         * @return a constant reference to the requested element
         * @throws std::out_of_range if the index is out of bounds
         */
        const_reference at(size_type index) const
        {
            if (index >= array_size) OUT_OF_BOUNDS_EXCEPTION

            return m_data[index];
        }
        
        /** @brief Returns the first element of the array `data[0]` */
        reference front() { return m_data[0]; }

        const_reference front() const { return m_data[0]; }

         /** @brief Returns the last element of the array `data[array_size - 1]` */
        reference back() { return m_data[array_size - 1]; }

        const_reference back() const { return m_data[array_size - 1]; }

        iterator begin() noexcept { return iterator(m_data); }
        
        iterator end() noexcept   { return iterator(m_data + array_size); }

        const_iterator cbegin() const noexcept { return const_iterator(m_data); }

        const_iterator cend() const noexcept   { return const_iterator(m_data + array_size); }
        
        reverse_iterator rbegin() noexcept { return reverse_iterator(m_data + array_size); }

        reverse_iterator rend() noexcept   { return reverse_iterator(m_data); }

        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(m_data + array_size); }

        const_reverse_iterator crend() const noexcept   { return const_reverse_iterator(m_data); }

        void fill(const value_type value)
        {
            for (size_type i = 0; i < array_size; ++i)
                m_data[i] = value;
        }

        void swap(array& arr) { stl::swap(arr.m_data, m_data); }

        /**
         * @brief Returns a reference to the element at specified location index;
         * @param index The location of the element
         * @return a reference to the requested element
         * @throws std::out_of_range if the index is out of bounds
         */
        reference operator[](size_type index)
        {
            if (index >= array_size) OUT_OF_BOUNDS_EXCEPTION

            return m_data[index];
        }

        /**
         * @brief Returns a constant reference to the element at specified location index;
         * @param index The location of the element
         * @return a constant reference to the requested element
         * @throws std::out_of_range if the index is out of bounds
         */
        constexpr const_reference operator[](size_type index) const
        {
            if (index >= array_size) OUT_OF_BOUNDS_EXCEPTION

            return m_data[index];
        }

        ~array() = default;

    private:
        T m_data[array_size];
    };

    /**
     * @brief Extracts Ith element from the array
     *        I must be an integer value in range [​0​, N). This is enforced at compile time as opposed to at() or operator[].
     * @param I   The index of the element
     * @param arr The array from which the contents are extracted 
     */
    template <unsigned int I, typename T, stl::size_t array_size> 
    constexpr T& get(array<T, array_size> &arr) { return arr[I]; }

    /**
     * @brief Swaps the contents of lhs and rhs by calling the swap member function.
     * @param lhs, rhs containers whose contents to swap
     */
    template <typename T, stl::size_t array_size>
    inline void swap(array<T, array_size> &lhs, array<T, array_size> &rhs) { lhs.swap(rhs); }
    
    /**
     * @brief Lexicographically compares the values of two arrays using the 'equal' function from core.h
     * @param lhs arrays whose contents to compare
     * @param rhs arrays whose contents to compare
     * @return true if the contents of the arrays are equal, false otherwise
     */
    template <typename T, stl::size_t array_size>
    inline bool operator==(const stl::array<T, array_size>& lhs, const stl::array<T, array_size>& rhs) { return stl::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin()); }

    /**
     * @brief Lexicographically compares the values of two arrays using the 'equal' function from core.h
     * @param lhs arrays whose contents to compare
     * @param rhs arrays whose contents to compare
     * @return true if the contents of the arrays are not equal, false otherwise
     */
    template <typename T, stl::size_t array_size>
    inline bool operator!=(const stl::array<T, array_size>& lhs, const stl::array<T, array_size>& rhs) { return !(lhs == rhs); }
    
    /**
     * @brief Lexicographically compares the values of two arrays using the 'lexicographical_compare' function from core.h
     * @param lhs arrays whose contents to compare
     * @param rhs arrays whose contents to compare
     * @return true if the contents of the lhs are lexicographically less than the contents of rhs, false otherwise
     */
    template <typename T, stl::size_t array_size>
    inline bool operator<(const stl::array<T, array_size>& lhs, const stl::array<T, array_size>& rhs) { return stl::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend()); }

    /**
     * @brief Lexicographically compares the values of two arrays using the 'lexicographical_compare' function from core.h
     * @param lhs arrays whose contents to compare
     * @param rhs arrays whose contents to compare
     * @return true if the contents of the lhs are lexicographically greater than the contents of rhs, false otherwise
     */
    template <typename T, stl::size_t array_size>
    inline bool operator>(const stl::array<T, array_size>& lhs, const stl::array<T, array_size>& rhs) { return rhs < lhs; }
    
    /**
     * @brief Lexicographically compares the values of two arrays using the 'lexicographical_compare' function from core.h
     * @param lhs arrays whose contents to compare
     * @param rhs arrays whose contents to compare
     * @return true if the contents of the lhs are lexicographically less than or equal to the contents of rhs, false otherwise
     */
    template <typename T, stl::size_t array_size>
    inline bool operator<=(const stl::array<T, array_size>& lhs, const stl::array<T, array_size>& rhs) { return !(lhs > rhs); }

    /**
     * @brief Lexicographically compares the values of two arrays using the 'lexicographical_compare' function from core.h
     * @param lhs arrays whose contents to compare
     * @param rhs arrays whose contents to compare
     * @return true if the contents of the lhs are lexicographically greater than or equal to the contents of rhs, false otherwise
     */
    template <typename T, stl::size_t array_size>
    inline bool operator>=(const stl::array<T, array_size>& lhs, const stl::array<T, array_size>& rhs) { return !(lhs < rhs); }
}

#endif // ARRAY_H