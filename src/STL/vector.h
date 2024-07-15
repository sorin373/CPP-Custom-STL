#ifndef __VECTOR_H__
#define __VECTOR_H__ 1

#include "../memory/memory.h" // memcpy and typedef
#include "reverse_iterator.h"

#include <initializer_list>
#include <stdlib.h>
#include <math.h>

#define INITIAL_VECTOR_SIZE 0
#define VECTOR_SIZE_ALLOCATOR(__l) (__l) * sizeof(T)
#define OUT_OF_BOUNDS_EXCEPTION throw std::out_of_range("Index out of bounds!\n");

namespace stl
{
    /**
     *  @brief Simple vector container. Automatic resizing with all the functionalies necessary
     */
    template <typename T> class vector
    {
        typedef size_t  size_type;
        typedef T       value_type;

        typedef reverse_iterator<value_type>       reverse_iterator;
        typedef const_reverse_iterator<value_type> const_reverse_iterator;

        typedef vector& vector_reference;

    public:
        typedef T* iterator;
        typedef T& reference;

        typedef const T* const_iterator;
        typedef const T& const_reference;

        vector() noexcept : m_capacity(0), m_size(0), m_data(nullptr) { }

        vector(std::initializer_list<value_type> init) : m_capacity(init.size()), m_size(init.size()), m_data(nullptr)
        {
            this->m_data = (T *)malloc(VECTOR_SIZE_ALLOCATOR(m_size));

            if (this->m_data == nullptr)
                throw std::runtime_error("malloc: couldn't allocate memory!\n");

            memcpy(m_data, init.begin(), VECTOR_SIZE_ALLOCATOR(m_size));
        }

        size_type size() const noexcept { return m_size; }

        uint64_t max_size() { return pow(2, ENV) / sizeof(value_type) - 1; }  // (2 ^ nativePointerBitWidth) / sizeof(dataType) - 1

        size_type capcacity() const noexcept { return m_capacity; }

        bool empty() const noexcept { return m_size == INITIAL_VECTOR_SIZE; } // 1 if empty, 0 if not

        void resize(size_type new_size)
        {
            if (new_size < m_size)
            {
                m_size = new_size;
                return;
            }
                
            m_capacity = new_size + new_size / 2 + 1;

            T *temp_ptr = (T *)malloc(VECTOR_SIZE_ALLOCATOR(m_capacity));

            if (temp_ptr == nullptr)
                throw std::runtime_error("malloc: couldn't allocate memory!\n");

            memcpy(temp_ptr, m_data, VECTOR_SIZE_ALLOCATOR(m_size - 1));

            free(m_data);

            m_data = temp_ptr;
        }

        void shrink_to_fit()
        {
            if (m_capacity == m_size)
                return;

            T *temp_ptr = (T *)malloc(VECTOR_SIZE_ALLOCATOR(m_size));

            if (temp_ptr == nullptr)
                throw std::runtime_error("malloc: couldn't allocate memory!\n");

            m_capacity = m_size;

            memcpy(temp_ptr, m_data, VECTOR_SIZE_ALLOCATOR(m_capacity));

            free(m_data);

            m_data = temp_ptr;
        }

        void reserve(size_type size)
        {
            if (size <= m_capacity)
                return;

            T *temp_ptr = (T *)malloc(VECTOR_SIZE_ALLOCATOR(size));

            if (temp_ptr == nullptr)
                throw std::runtime_error("malloc: couldn't allocate memory!\n");

            m_capacity = size;

            memcpy(temp_ptr, m_data, VECTOR_SIZE_ALLOCATOR(m_size));

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

            size_type count = 0;

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

            m_data = (value_type *)malloc(VECTOR_SIZE_ALLOCATOR(m_size));

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

            m_data = (value_type *)malloc(VECTOR_SIZE_ALLOCATOR(size));

            if (m_data == nullptr)
                throw std::runtime_error("malloc: couldn't allocate memory!\n");

            memcpy(m_data, first, VECTOR_SIZE_ALLOCATOR(size));

            m_size = m_capacity = size;
        }

        void assign(std::initializer_list<value_type> init)
        { 
            m_size = m_capacity = init.size();

            m_data = (T *)malloc(VECTOR_SIZE_ALLOCATOR(m_size));

            if (m_data == nullptr)
                throw std::runtime_error("malloc: couldn't allocate memory!\n");

            memcpy(m_data, init.begin(), VECTOR_SIZE_ALLOCATOR(m_size));
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

                m_data = (T *)malloc(VECTOR_SIZE_ALLOCATOR(m_size));

                if (m_data == nullptr)
                    throw std::runtime_error("malloc: couldn't allocate memory!\n");

                memcpy(this->m_data, other.cbegin(), VECTOR_SIZE_ALLOCATOR(m_size));
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