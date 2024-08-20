#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "core.h"
#include "../memory/memory.h"
#include "reverse_iterator.h"

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
    template <typename T, unsigned int array_size> class array
    {
        typedef       T           value_type;
        typedef       value_type* pointer;
        typedef const value_type* const_pointer;

        typedef unsigned int size_type;

        constexpr static int __SIZE__ = array_size * META_SIZE;

    public:

        typedef T*       iterator;
        typedef const T* const_iterator;

        typedef T&       reference;
        typedef const T& const_reference;

        typedef reverse_iterator<value_type>       reverse_iterator;
        typedef const_reverse_iterator<value_type> const_reverse_iterator;

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

            memcpy(m_data, init.begin(), __SIZE__);
        }

        array& operator=(const std::initializer_list<value_type> init)
        {
            if (init.size() > array_size) MEMORY_OVERFLOW_EXCEPTION

            memcpy(m_data, init.begin(), array_size * META_SIZE);            
        }

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

        constexpr bool empty() noexcept { return array_size == 0; }

        pointer data() noexcept { return m_data; } 

        const_pointer data() const noexcept { return m_data; }  

        /**
         * @brief Returns a reference to an element within the container
         * @param index The location of the element
         */
        reference at(size_type index) 
        { 
            if (index >= array_size) OUT_OF_BOUNDS_EXCEPTION

            return m_data[index]; 
        }

        /**
         * @brief Returns a constant reference to an element within the container
         * @param index The location of the element
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

        iterator begin() { return iterator(m_data); }
        
        iterator end()   { return iterator(m_data + array_size); }

        const_iterator cbegin() const noexcept { return const_iterator(m_data); }

        const_iterator cend() const noexcept   { return const_iterator(m_data + array_size); }
        
        reverse_iterator rbegin() { return reverse_iterator(m_data + array_size); }

        reverse_iterator rend()   { return reverse_iterator(m_data); }

        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(m_data + array_size); }

        const_reverse_iterator crend() const noexcept   { return const_reverse_iterator(m_data); }

        void fill(const value_type value)
        {
            for (size_type i = 0; i < array_size; ++i)
                m_data[i] = value;
        }

        void swap(array& arr)
        {
            iterator temp_m_data = m_data;

            m_data = arr.m_data;

            arr.m_data = temp_m_data;
        }

        /**
         * @brief Returns a reference to the element at specified location index;
         * @param index The location of the element
         */
        reference operator[](size_type index)
        {
            if (index >= array_size) OUT_OF_BOUNDS_EXCEPTION

            return m_data[index];
        }

        /**
         * @brief Returns a constant reference to the element at specified location index;
         * @param index The location of the element
         */
        constexpr const_reference operator[](size_type index) const
        {
            if (index >= array_size) OUT_OF_BOUNDS_EXCEPTION

            return m_data[index];
        }

        ~array() = default;

    private:
        value_type m_data[array_size];
    };

    /**
     * @brief Extracts Ith element from the array
     *        I must be an integer value in range [​0​, N). This is enforced at compile time as opposed to at() or operator[].
     * @param I   The index of the element
     * @param arr The array from which the contents are extracted 
     */
    template <unsigned int I, typename T, unsigned int array_size> 
    constexpr T& get(array<T, array_size> &arr)
    {
        if (I >= array_size) OUT_OF_BOUNDS_EXCEPTION

        return arr[I];
    }

    /**
     * @brief Swaps the contents of lhs and rhs by calling the swap member function.
     * @param lhs, rhs containers whose contents to swap
     */
    template <typename T, unsigned int array_size>
    inline void swap(array<T, array_size> &lhs, array<T, array_size> &rhs) { lhs.swap(rhs); }

    template <typename T, unsigned int array_size>
    inline bool operator==(const stl::array<T, array_size>& lhs, const stl::array<T, array_size>& rhs) { return stl::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin()); }

    template <typename T, unsigned int array_size>
    inline bool operator!=(const stl::array<T, array_size>& lhs, const stl::array<T, array_size>& rhs) { return !(lhs == rhs); }

    template <typename T, unsigned int array_size>
    inline bool operator<(const stl::array<T, array_size>& lhs, const stl::array<T, array_size>& rhs) { return stl::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend()); }

    template <typename T, unsigned int array_size>
    inline bool operator>(const stl::array<T, array_size>& lhs, const stl::array<T, array_size>& rhs) { return rhs < lhs; }

    template <typename T, unsigned int array_size>
    inline bool operator<=(const stl::array<T, array_size>& lhs, const stl::array<T, array_size>& rhs) { return !(lhs > rhs); }

    template <typename T, unsigned int array_size>
    inline bool operator>=(const stl::array<T, array_size>& lhs, const stl::array<T, array_size>& rhs) { return !(lhs < rhs); }
}

#endif // ARRAY_H