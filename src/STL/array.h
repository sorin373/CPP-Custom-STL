#ifndef __ARRAY_H__
#define __ARRAY_H__ 1

#include "../memory/memory.h"
#include "reverse_iterator.h"

#define OUT_OF_BOUNDS_EXCEPTION throw std::out_of_range("Index out of bounds!\n");

namespace stl
{
    /**
     * @brief This is a container that encapsulates fixed size arrays.
     * 
     * @param T          Type of element
     * @param array_size The fixed no. of elements that the array can have
     */
    template <typename T, size_t array_size> class array
    {
        typedef       T           value_type;
        typedef       value_type* pointer;
        typedef const value_type* const_pointer;

        // Private aliases for the reverse iterators. The user will use the generic reverse_iterator template.
        typedef reverse_iterator<value_type>       reverse_iterator;
        typedef const_reverse_iterator<value_type> const_reverse_iterator;

        typedef size_t size_type;

    public:
        /**
         * @brief Define type aliases for the iterators types.
         *        These iterators provide random access capabilities and can be used to traverse containers that store elements of value_type
         * 
         * @example 
         * for (iterator it = array.begin(); it != array.end(); it++) {
         *   Access each element using *it
         * }
         */
        typedef value_type*     iterator;
        typedef const iterator  const_iterator;

        /**
         * @brief Define type aliases for the references types.
         *        They provide a way to refer to elements within the container and can be used to access or modify the element.
         */
        typedef value_type&     reference;
        typedef const reference const_reference;

        array() : m_data(nullptr)
        {
            m_data = (value_type *)malloc(array_size * sizeof(value_type));

            if (m_data == nullptr)
                throw std::runtime_error("malloc: couldn't allocate memory!\n");
        }

        array(std::initializer_list<value_type> init) : m_data(nullptr)
        {
            if (init.size() > array_size)
                throw std::runtime_error("Too many elements. Check the array size to be correct!\n");

            m_data = (value_type *)malloc(array_size * sizeof(value_type));

            if (m_data == nullptr)
                throw std::runtime_error("malloc: couldn't allocate memory!\n");

            memcpy(m_data, init.begin(), array_size * sizeof(value_type));               // this does not set the rest of the elements to 0 (std::array does set the rest to 0)
        }

        constexpr size_type max_size() const noexcept { return array_size; }

        pointer data() noexcept { return this->m_data; }        

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

        reference front()
        {
            if (array_size > 0)
                return m_data[0];
        }

        reference back()
        {
            if (array_size > 0)
                return m_data[array_size - 1];
        }

        iterator begin() { return this->m_data; }
        
        iterator end()   { return this->m_data + array_size; }

        const_iterator cbegin() const noexcept { return this->m_data; }

        const_iterator cend() const noexcept   { return this->m_data + array_size; }
        
        reverse_iterator rbegin() { return reverse_iterator(this->m_data + array_size); }

        reverse_iterator rend()   { return reverse_iterator(this->m_data); }

        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(m_data + array_size); }

        const_reverse_iterator crend() const noexcept   { return const_reverse_iterator(m_data); }

        void fill(const value_type value)
        {
            for (size_type i = 0; i < array_size; i++)
                m_data[i] = value;
        }

        void swap(array &arr)
        {
            iterator temp_m_data = this->m_data;

            this->m_data = arr.m_data;

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
        const_reference operator[](size_type index) const
        {
            if (index >= array_size) OUT_OF_BOUNDS_EXCEPTION

            return m_data[index];
        }

        bool operator==(const array other)
        {
            /**
             * The compiler won't allow arrays with different sizes to be compared. As a result this if statement is unnecessary!!
             
            if (array_size != other.size())
                return false;
            */

            for (size_type i = 0; i < array_size; i++)
                if (this->m_data[i] != other.m_data[i])
                    return false;

            return true;
        }

        bool operator!=(const array other)
        {
            // same logic as '==' operator

            return !(this->m_data == other.m_data);
        }

        bool operator<(const array other)
        {
            size_type i = array_size;

            while (i != 0)
            {
                int c_var_1 = (int)(this->m_data[i]);
                int c_var_2 = (int)(other.m_data[i]);

                return (c_var_1 - c_var_2) < 0;

                i--;
            }
        }

        bool operator>(const array other)  { return !(this->m_data < other.m_data); }

        bool operator>=(const array other) { return !(this->m_data < other.m_data); }

        bool operator<=(const array other) { return !(this->m_data > other.m_data); }

    private:
        value_type *m_data;
    };

    /**
     * @brief Extracts Ith  element from the array
     *        I must be an integer value in range [​0​, N). This is enforced at compile time as opposed to at() or operator[].
     * @param I   The index of the element
     * @param arr The array from which the contents are extracted 
     */
    template <size_t I, typename T, size_t array_size> 
    constexpr T& get(array<T, array_size> &arr)
    {
        if (I >= array_size) OUT_OF_BOUNDS_EXCEPTION

        return arr[I];
    }

    /**
     * @brief Swaps the contents of lhs and rhs by calling the swap member function.
     * @param lhs, rhs containers whose contents to swap
     */
    template <typename T, size_t array_size>
    inline void swap(array<T, array_size> &lhs, array<T, array_size> &rhs) { lhs.swap(rhs); }
}

#endif