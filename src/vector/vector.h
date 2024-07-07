#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "../memory/memory.h"

#include <stdlib.h>
#include <math.h>

#if _WIN32 || _WIN64
   #if _WIN64
     #define ENV 64
  #else
    #define ENV 32
  #endif
#endif

#if __GNUC__
  #if __x86_64__ || __ppc64__
    #define ENV 64
  #else
    #define ENV 32
  #endif
#endif

#define INITIAL_VECTOR_SIZE 0

namespace stl
{
    template <typename T> class vector
    {
    private:
        T     *m_data;
        size_t m_size;
        size_t m_capacity;
        
    public:

        /**
         *  @brief iterators declaration
         */
        typedef T* iterator;
        typedef T& reference;

        typedef const T* const_iterator;
        typedef const T& const_reference;

        explicit vector(size_t size = INITIAL_VECTOR_SIZE)
        {
            this->m_capacity = size;

            this->m_size = size;

            this->m_data = (T *)malloc(size * sizeof(T));

            if (this->m_data == nullptr)
                throw "malloc: Couldn't allocate memory!\n";
        }

        size_t size() const noexcept { return m_size; }

        uint64_t max_size() { return pow(2, ENV) / sizeof(T) - 1; } // (2 ^ nativePointerBitWidth) / sizeof(dataType) - 1

        size_t capcacity() const noexcept { return m_capacity; }

        bool empty() const noexcept { return m_size == INITIAL_VECTOR_SIZE; } // 1 if empty, 0 if not

        void resize(size_t new_size)
        {
            if (new_size < m_size)
            {
                m_size = new_size;
                return;
            }
                
            m_capacity = new_size + new_size / 2 + 1;

            T *temp_ptr = (T *)malloc(m_capacity * sizeof(T));

            if (temp_ptr == nullptr)
                throw std::runtime_error("malloc: couldn't allocate memory!\n");

            memcpy(temp_ptr, m_data, (m_size - 1) * sizeof(T));

            free(m_data);

            m_data = temp_ptr;
        }

        void shrink_to_fit()
        {
            if (m_capacity == m_size)
                return;

            m_capacity = m_size;

            T *temp_ptr = (T *)malloc(m_capacity * sizeof(T));

            if (temp_ptr == nullptr)
                throw std::runtime_error("malloc: couldn't allocate memory!\n");

            memcpy(temp_ptr, m_data, m_capacity * sizeof(T));

            free(m_data);

            m_data = temp_ptr;
        }

        void reserve(size_t size)
        {
            if (size <= m_capacity)
                return;

            m_capacity = size;

            T *temp_ptr = (T *)malloc(m_capacity * sizeof(T));

            if (temp_ptr == nullptr)
                throw std::runtime_error("malloc: couldn't allocate memory!\n");

            memcpy(temp_ptr, m_data, m_size * sizeof(T));

            free(m_data);

            m_data = temp_ptr;
        }

        void pushback(T element)
        {
            m_size++;

            if (m_size > m_capacity)
                resize(m_size);

            m_data[m_size - 1] = element;
        }

        reference at(size_t index) 
        { 
            if (index >= m_capacity)
                throw std::out_of_range("Index out of bounds!");

            return m_data[index]; 
        }

        reference front() noexcept { return m_data[0]; }

        reference back() noexcept { return m_data[m_size - 1]; }

        iterator data() noexcept { return m_data; }

        void assign(size_t size, T value)
        {
            
        }

        iterator begin() noexcept              { return m_data; }

        const_iterator cbegin() const noexcept { return m_data; }

        iterator end() noexcept                { return m_data + m_size; }

        const_iterator cend() const noexcept   { return m_data + m_size; }

        class reverse_iterator
        {
        private:
            iterator m_ptr;

        public:
            reverse_iterator(iterator ptr) { this->m_ptr = ptr; }

            iterator operator->() { return m_ptr; }

            reference operator*() const { return *m_ptr; }

            reverse_iterator& operator++() // prefix
            {
                --m_ptr;
                return *this;
            }

            reverse_iterator operator++(int) // postfix
            {
                reverse_iterator tmp = *this;

                m_ptr--;

                return tmp;
            }

            bool operator==(const reverse_iterator &other) const { return m_ptr == other.m_ptr; }

            bool operator!=(const reverse_iterator &other) const { return m_ptr != other.m_ptr; }
        };

        class const_reverse_iterator
        {
        private:
            const_iterator m_ptr;
            
        public:
            const_reverse_iterator(const_iterator ptr) { this->m_ptr = ptr; }

            const_iterator operator->() const { return m_ptr; }

            const_reference operator*() const { return *m_ptr; }

            const_reverse_iterator& operator++()
            {
                --m_ptr;
                return *this;
            }

            const_reverse_iterator operator++(int)
            {
                const_reverse_iterator tmp = *this;

                m_ptr--;

                return tmp;
            }

            bool operator==(const const_reverse_iterator &other) const { return m_ptr == other.m_ptr; }
            
            bool operator!=(const const_reverse_iterator &other) const { return m_ptr != other.m_ptr; }
        };

        reverse_iterator rbegin() { return reverse_iterator(m_data + m_size - 1); }

        reverse_iterator rend() { return reverse_iterator(m_data -1); }

        const_reverse_iterator crbegin() { return const_reverse_iterator(m_data + m_size - 1); }

        const_reverse_iterator crend() { return const_reverse_iterator(m_data - 1); } 

        reference operator[](size_t index) 
        { 
            if (index >= m_capacity)
                throw std::out_of_range("Index out of bounds!");

            return m_data[index];
        }

        const_reference operator[](size_t index) const 
        {
            if (index >= m_capacity)
                throw std::out_of_range("Index out of bounds!");

            return m_data[index];
        }

        ~vector() { free(m_data); }
    };
}

#endif // VECTOR_H