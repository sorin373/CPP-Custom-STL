#ifndef __ARRAY_H__
#define __ARRAY_H__ 1

#include "../memory/memory.h"

#define OUT_OF_BOUNDS_EXCEPTION throw std::out_of_range("Index out of bounds!\n");

namespace stl
{
    template <typename T, size_t array_size> class array
    {
        typedef T                 value_type;
        typedef       value_type* pointer;
        typedef const value_type* const_pointer;

        typedef size_t            size_type;

    public:
        typedef value_type* iterator;
        typedef value_type& reference;

        typedef const iterator  const_iterator;
        typedef const reference const_reference;

        array() : m_data(nullptr), m_size(0)
        {
            m_data = (value_type *)malloc(array_size * sizeof(value_type));

            if (m_data == nullptr)
                throw std::runtime_error("malloc: couldn't allocate memory!\n");
        }

        array(std::initializer_list<value_type> init) : m_data(nullptr), m_size(0)
        {
            if (init.size() > array_size)
                throw std::runtime_error("Too many elements. Check the array size to be correct!\n");

            m_data = (value_type *)malloc(array_size * sizeof(value_type));

            if (m_data == nullptr)
                throw std::runtime_error("malloc: couldn't allocate memory!\n");

            memcpy(m_data, init.begin(), array_size * sizeof(value_type));               // this does not set the rest of the elements to 0 (std::array does set the rest to 0)

            m_size = init.size();
        }

        constexpr size_type max_size() const noexcept { return array_size; }

        size_type size() const noexcept               { return this->m_size; }

        pointer data() noexcept                       { return this->m_data; }

        const_pointer data() const noexcept           { return this->m_data; }

        value_type at(size_type index) 
        { 
            if (index >= array_size) OUT_OF_BOUNDS_EXCEPTION

            return m_data[index]; 
        }

        reference front() noexcept
        {
            if (array_size > 0)
                return m_data[0];
        }

        reference back() noexcept
        {
            if (array_size > 0)
                return m_data[m_size - 1];
        }

        iterator begin() noexcept { return m_data; }

        iterator end() noexcept { return m_data + array_size; } // still confuses if end should point to the last element or the last address in the array.
                                                                // Brings me to question the size() and max_size() functions in the std::array stl lib which both seem to return the array_size
                                                                // which effectively means the full capacity of the array. Having said that, I do believe that the user should count the ammount of elements 
                                                                // added to the array since automatic resizing for this stl is out of the question and only one memeber function should return the max_size
                                                                // (which is kind of stupid considering that the user acutally sets up that value...).

                                                                // Might be a good idea to start with a capacity = array_size just like the vector stl and resize until the max_size reaches that capacity.

        void fill(const value_type value) noexcept
        {
            for (size_type i = 0; i < array_size; i++)
                m_data[i] = value;

            m_size = array_size;
        }

        void swap(array &arr) // array size is the same hopefully
        {
            iterator temp_m_data = this->m_data;

            this->m_data = arr.m_data;

            arr.m_data = temp_m_data;
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
        size_type   m_size; // contains the real array size != max_size (problem: can not be calculated unless the constructor with the initializer list is used)
                            // the std::array does not have this member variable
    };
}

#endif