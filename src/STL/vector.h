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

    @file vector.h

    @brief This is part of the standard template libraries (STL) and it is used to implement resizeable arrays
           and commonly used algorithms.
 */

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "allocator.h"
#include "reverse_iterator.h"
#include "core.h"

#include <initializer_list>
#include <stdexcept>
#include <math.h>

#if defined(_WIN32) || defined(_WIN64)
  #if defined(_WIN64)
    #define ENV 64
  #else
    #define ENV 32
  #endif
#elif defined(__GNUC__)
  #if defined(__x86_64__) || defined(__ppc64__)
    #define ENV 64
  #else
    #define ENV 32
  #endif
#else
  #error "Unknown platform"
#endif

#define OUT_OF_BOUNDS_EXCEPTION throw std::out_of_range("Index out of bounds!\n");
#define META_SIZE               sizeof(T)

namespace stl
{
    /**
     *  @brief %vector (STL) container (a dynamic C-style array). It adds extra functionalites to the basic CPP arrays, by resizing the memory when necessary.
     *         In addition it consists in many member functions that help the user manage the array data faster and more efficiently. 
     *         Subscripting ( @c [] ) access is also provided as with C-style arrays.
     *  @param T The type of the elements.
     *  @param Allocator An allocator that is used to acquire/release memory and to construct/destroy the elements in that memory.
     */
    template <
        typename T, 
        typename Allocator = allocator<T>
    > class vector
    {
    public:

        /***************
        * MEMBER TYPES *
        ***************/

        typedef T                 value_type;
        typedef Allocator         allocator_type;
        typedef stl::size_t       size_type;
        typedef stl::ptrdiff_t    difference_type;

        typedef T*          iterator;
        typedef const T*    const_iterator;

        typedef T&          reference;
        typedef const T&    const_reference;

        typedef reverse_iterator<value_type>          reverse_iterator;
        typedef const_reverse_iterator<value_type>    const_reverse_iterator;

        /**********************************
        * MEMBER FUNCTIONS - CONSTRUCTORS *
        **********************************/
       
        /**
         * @brief Creates an empty container
         * 
         * @example stl::vector<Type> my_vector;
         */
        vector() noexcept 
            : m_capacity(0), m_size(0), m_data(nullptr) { }
        
        /**
         * @brief Creates an empty container with a given allocator
         * @param alloc allocator to use for all memory allocations of this container
         * 
         * @example stl::vector<Type> my_vector(my_custom_allocator);
         */
        explicit vector(const allocator_type& alloc) 
            : m_alloc(alloc), m_capacity(0), m_size(0), m_data(nullptr) { }

        /**
         * @brief Constructs the container with count copies of elements with value value
         * @param count the size of the container @param value the value to initialize elements of the container with @param alloc allocator to use for all memory allocations of this container
         * 
         * @example stl::vector<int> my_vector(5);                                            // Creates a vector with 5 elements, each initialized to 0 (default constructor of int).
         * @example stl::vector<int> my_vector(5, 10);                                        // Creates a vector with 5 elements, each initialized to 10.
         * @example stl::vector<int, MyAllocator<int>> my_vector(5, 42, my_allocator<int>()); // Creates a vector with 5 elements, each initialized to 42 using the custom allocator 
         * 
         * @note  If T is a primitive type, such as int, T() would be 0
         * @throw If the allocator throws the std::bad_alloc exception, all allocated (constructed) elements up to that point will be destroyed and the memory will be freed.
         */
        explicit vector(size_type count, const T& value = T(), const Allocator& alloc = Allocator())
            : m_capacity(count), m_size(count), m_alloc(alloc), m_data(nullptr)
        {
            if (count > 0)
            {
                m_data = m_alloc.allocate(count);

                try
                {
                    for (size_type i = 0; i < m_size; ++i)
                        m_alloc.construct(m_data + i, value);
                }
                catch(...)
                {
                    for (size_type i = 0; i < m_size; ++i)
                        m_alloc.destroy(m_data + i);

                    m_alloc.deallocate(m_data, m_capacity);
                    throw;
                }
            }
        }

        /**
         * @brief Constructs the container using an initializer list
         * @param init 	initializer list to initialize the elements of the container with @param alloc allocator to use for all memory allocations of this container
         * 
         * @example stl::vector<std::string> my_vector({"the", "frogurt", "is", "also", "cursed"});
         * @example stl::vector<std::string> my_vector({"the", "frogurt", "is", "also", "cursed"}, my_allocator<std::string>());
         * 
         * @throw If the allocator throws the std::bad_alloc exception, all allocated (constructed) elements up to that point will be destroyed and the memory will be freed.
        */
        vector(std::initializer_list<value_type> init, const Allocator& alloc = Allocator())
            : m_alloc(alloc), m_size(init.size()), m_capacity(init.size()), m_data(nullptr)
        {
            if (m_size > 0)
            {
                m_data = m_alloc.allocate(m_size);

                try
                {
                    // constant iterator for the initializer list - std::initializer_list<T>::begin() returns a constant pointer
                    const value_type* it = init.begin();    

                    for (size_type i = 0; i < m_size; ++i)
                    {
                        m_alloc.construct(m_data + i, *it);
                        ++it;
                    }
                }
                catch(...)
                {
                    for (size_type i = 0; i < m_size; ++i)
                        m_alloc.destroy(m_data + i);

                    m_alloc.deallocate(m_data, m_capacity);
                    throw;
                }
            }   
        }

        /**
         * @brief Constructs the container using another %vector container
         * @param other another container to be used as source to initialize the elements of the container with
         * 
         * @example stl::vector<int> my_vector_init;                // empty vector
         *          stl::vector<int> my_new_vector(my_vector_init); // construct a new vector using the empty vector 
         * 
         * @throw If the allocator throws the std::bad_alloc exception, all allocated (constructed) elements up to that point will be destroyed and the memory will be freed.
        */
        vector(const vector& other) 
            : m_alloc(other.m_alloc), m_size(other.m_size), m_capacity(other.m_capacity), m_data(nullptr) 
        {
            if (!other.empty())
            {
                m_data = m_alloc.allocate(m_capacity);

                try
                {
                    for (size_type i = 0; i < m_size; ++i)
                        m_alloc.construct(m_data + i, other.m_data[i]);
                }
                catch(...)
                {
                    for (size_type i = 0; i < m_size; ++i)
                        m_alloc.destroy(m_data + i);

                    m_alloc.deallocate(m_data, m_capacity);
                    throw;
                }
            }    
        }

        /**
         * @brief Constructs the container using another %vector container and allocator (if one is provided)
         * @param other another container to be used as source to initialize the elements of the container with @param alloc allocator to use for all memory allocations of this container
         * 
         * @example stl::vector<Type> my_vector_init;                                       // empty vector
         *          stl::vector<Type> my_new_vector(my_vector_init);                        // construct a new vector using the empty vector 
         *          stl::vector<Type> my_new_vector_2(my_vector_init, my_allocator<Type>()) // construct a new vector using the empty vector and a custom memory allocator
         * 
         * @throw If the allocator throws the std::bad_alloc exception, all allocated (constructed) elements up to that point will be destroyed and the memory will be freed.
        */
        vector(const vector& other, const Allocator& alloc) 
            : m_alloc(alloc), m_size(other.size()), m_capacity(other.capcacity()), m_data(nullptr)
        {
            if (!other.empty())
            {
                m_data = m_alloc.allocate(m_size);

                try
                {
                    for (size_type i = 0; i < m_size; ++i)
                        m_alloc.construct(m_data + i, other.m_data[i]);
                        
                }
                catch(...)
                {
                    for (size_type i = 0; i < m_size; ++i)
                        m_alloc.destroy(m_data + i);

                    m_alloc.deallocate(m_data, m_capacity);
                    throw;
                }
            }
        }

        /*************
        * DESTRUCTOR *
        *************/

        /**
         * @brief Destructs the vector. The destructors of the elements are called and the used storage is deallocated. 
         * @note  if the elements are pointers, the pointed-to objects are not destroyed.
         */
        ~vector() 
        {
            if (m_data != nullptr) 
            {
                for (size_type i = 0; i < m_size; ++i)
                    m_alloc.destroy(m_data + i);

                m_alloc.deallocate(m_data, m_capacity);
            }
        }

        /**
         * @brief Copy assignment operator. Replaces the contents with a copy of the contents of other.
         * @param other another container to use as data source
         * @return *this
         * 
         * @example stl::vector<int> x({1, 2, 3, 4}), y, z;
         *          y = x;
         */
        vector& operator=(const vector& other) 
        { 
            if (this != &other)
            {
                if (m_data != nullptr)
                    m_alloc.deallocate(m_data, m_capacity);

                m_capacity = other.capcacity();
                m_size     = other.size();
                m_alloc    = other.get_allocator();

                m_data = m_alloc.allocate(m_size);

                for (size_type i = 0; i < m_size; ++i)
                    m_alloc.construct(m_data + i, other[i]);
            }

            return *this;
        }

        /**
         * @brief Replaces the contents with those identified by initializer list `init`
         * @param init initializer list to use as data source
         * @return *this
         * 
         * @example const auto x = {5, 6, 7, 8};
         *          stl::vector<int> y;
         * 
         *          y = x;
         */
        vector& operator=(const std::initializer_list<T> init)
        {
            if (m_data != nullptr)
                m_alloc.deallocate(m_data, m_capacity);

            m_capacity = m_size = init.size();

            m_data = m_alloc.allocate(m_size);

            const value_type* it = init.begin();

            for (size_type i = 0; i < m_size; ++i)
            {
                m_alloc.construct(m_data + i, *it);
                ++it;
            }
        }

        /**
         * @brief Replaces the contents with `size` copies of value `value`
         * @param size the new size of the container @param value the value to initialize elements of the container with
         * 
         * @example stl::vector<char> characters;
         *          characters.assign(5, 'a');
         */
        void assign(size_type size, const reference value)
        {
            if (size > 0)
            {
                m_size = m_capacity = size;
                m_data = m_alloc.allocate(m_size);

                try
                {
                    for (size_type i = 0; i < m_size; ++i)
                        m_alloc.construct(m_data + i, value);
                }
                catch(...)
                {
                    for (size_type i = 0; i < m_size; ++i)
                        m_alloc.destroy(m_data + i);

                    m_alloc.deallocate(m_data, m_capacity);
                }
            }
        }

        /**
         * @brief Replaces the contents with copies of those in the range [`first`, `last`).
         * @param first begining iterator @param last end iterator
         * 
         * @example const std::string extra(6, 'b');
         *          stl::vector<char> characters;
         *          
         *          characters.assign(extra.begin(), extra.end());
         */
        template <typename InputIT>
        void assign(InputIT first, InputIT last)
        {
            if (first == nullptr || last == nullptr)
                return;

            int size = first - last;

            if (size < 0) size = -size;

            m_data = m_alloc.allocate(size);

            try
            {
                for (size_type i = 0; i < size; ++i)
                {   
                    m_alloc.construct(m_data + i, *first);
                    ++first;
                }   
            }
            catch(...)
            {
                for (size_type i = 0; i < size; ++i)
                    m_alloc.destroy(m_data + i);

                m_alloc.deallocate(m_data, size);
                throw;
            }

            m_size = m_capacity = size;
        }

        /*################################################## __TO DO__ ##################################################*/

        void assign(std::initializer_list<value_type> init)
        { 
            m_size = m_capacity = init.size();

            m_data = m_alloc.allocate(m_size);

            memcpy(m_data, init.begin(), m_size * META_SIZE);
        }

        /** @returns The number of elements currently present in the vector. */
        constexpr size_type size() const noexcept { return m_size; }

        /**
         * @brief (2 ^ nativePointerBitWidth) / sizeof(dataType) - 1
         * @returns The theoretical maximum number of items that could be put in the vector.
         */
        constexpr uint64_t max_size() { return pow(2, ENV) / META_SIZE - 1; }
        
        constexpr size_type capcacity() const noexcept { return m_capacity; }

        /** @returns Returns true if the vector has no elements. */
        bool empty() const noexcept { return m_size == 0; }

        /**
         * @brief This function resizes the container. While the m_size increses by 1 the m_capacity is calculated 
         *        to be >= to m_size so that the container is resized efficently. The formula I came up with is:
         *        m_capacity = new_size + new_size / 2 + 1. It is designed to grow as the new_size gets bigger and bigger,
         *        balancing the need of resizes.
         * @param new_size The new size to which the vector will be resized to
         * @throw If the new memory block can not be allocated, a runtime error will be thrown.
         */
        void resize(size_type new_size)
        {
            if (new_size < m_size)
            {
                for (size_type i = new_size; i < m_size; ++i)
                    m_alloc.destroy(m_data + i);

                m_size = new_size;
                return;
            }
                
            size_type new_capacity = new_size + new_size / 2 + 1;               

            value_type* temp_ptr = m_alloc.allocate(new_capacity);

            // memcpy can be used because the objects are already constructed
            memcpy(temp_ptr, m_data, (m_size - 1) * META_SIZE);

            m_alloc.deallocate(m_data, m_capacity);

            m_capacity = new_capacity;
            m_data = temp_ptr;
        }

        /**
         * @brief This function frees any memory which is not used by the vector. In other words, the m_capacity will equal to m_size.
         * @throw If the new memory block can not be allocated, a runtime error will be thrown.
         */
        void shrink_to_fit()
        {
            if (m_capacity == m_size)
                return;

            value_type *temp_ptr = m_alloc.allocate(m_size);

            m_capacity = m_size;

            // memcpy can be used because the objects are already constructed
            memcpy(temp_ptr, m_data, m_capacity * META_SIZE);

            m_alloc.deallocate(m_data, m_capacity);

            m_data = temp_ptr;
        }

        /**
         * @brief Increase the capacity of the vector (the total number of elements that the vector can hold without requiring reallocation)
         *        reserve() does not change the size of the vector.
         * @param new_cap new capacity of the vector, in number of elements
         * @throw If the new memory block can not be allocated, a runtime error will be thrown.
         */
        void reserve(size_type new_cap)
        {
            if (new_cap > max_size())
                throw std::length_error();

            if (new_cap <= m_capacity)
                return;

            value_type *temp_ptr = m_alloc.allocate(new_cap);

            m_capacity = new_cap;

            m_alloc.deallocate(m_data, m_capacity);

            m_data = temp_ptr;
        }

        void push_back(reference element)
        {
            ++m_size;

            if (m_size > m_capacity)
                resize(m_size);

            m_alloc.construct(&m_data[m_size - 1], element);
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
            int index = get_index(position, begin());

            if (index >= m_size) OUT_OF_BOUNDS_EXCEPTION

            m_size++;

            if (m_size > m_capacity)
                resize(m_size);

            for (size_type i = size() - 1; i >= index; i--)
                m_data[i + 1] = m_data[i];
        
            m_data[index] = value;
        }

        void insert(const_iterator position, size_type size, value_type value)
        {
            size_type index = get_index(position, begin());

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
            int index = get_index(position, begin());

            if (index >= m_size) OUT_OF_BOUNDS_EXCEPTION

            int additional_size = std::abs(it_1 - it_2);

            size_type old_size = m_size;

            m_size += additional_size;

            if (m_size > m_capacity)
                resize(m_size);

            for (size_type i = old_size - 1; i >= index; --i)
                m_data[i + additional_size] = m_data[i];

            for (size_type i = index; i <= index + additional_size - 1; ++i)
                m_data[i] = *it_1++;
        }

        void clear() noexcept
        { 
            if (m_size == 0)
                return;

            for (size_type i = 0, n = size(); i < n; ++i) 
                m_alloc.destroy(&m_data[i]);

            m_size = 0;
        }

        void erase(iterator position)
        {
            if (position >= end()) OUT_OF_BOUNDS_EXCEPTION

            for (iterator it = position; it != end() - 1; it++)
                *it = *(it + 1);

            --m_size;
        }

        void erase(iterator begin, iterator end)
        {
            if (begin >= end() || end >= end()) OUT_OF_BOUNDS_EXCEPTION

            if (begin == end)
            {
                erase(begin);
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

            for (iterator it = begin; it != end && it_original != end(); it++)
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
            int index = get_index(begin(), position); // int type bec for some reason the for breaks if unsigned int 
            size_type old_size = m_size;

            if (index >= m_size) OUT_OF_BOUNDS_EXCEPTION

            m_size++;

            if (m_size > m_capacity)
                resize(m_size);

            for (int i = old_size - 1; i >= index; i--)
                m_data[i + 1] = m_data[i];

            new (&m_data[index]) value_type(std::forward<Args>(args)...);

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

        reference at(size_type pos) { return m_data[pos]; }

        const_reference at(size_type pos) const { return m_data[pos]; }

        reference front() noexcept { return m_data[0]; }

        reference back() noexcept  { return m_data[m_size - 1]; }

        iterator data() noexcept   { return m_data; }

        iterator begin()                       { return iterator(m_data); }

        const_iterator cbegin() const noexcept { return const_iterator(m_data); }

        iterator end() noexcept                { return iterator(m_data + m_size); }

        const_iterator cend() const noexcept   { return const_iterator(m_data + m_size); }

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

        

        allocator_type get_allocator() const { return m_alloc; }

        

    private:
        value_type*    m_data;
        size_type      m_size;
        size_type      m_capacity;
        allocator_type m_alloc;

        constexpr size_type get_index(iterator it_1, iterator it_2) const noexcept { return std::abs(it_1 - it_2); }
    };

    template <typename T, typename Alloc>
    inline void swap(vector<T, Alloc>& lhs, vector<T, Alloc>& rhs) { lhs.swap(rhs); }

    template <typename T, typename Alloc>
    inline bool operator==(const stl::vector<T, Alloc>& lhs, const stl::vector<T, Alloc>& rhs) { return (lhs.size() == rhs.size()) && stl::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin()); }

    template <typename T, typename Alloc>
    inline bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) { return !(lhs == rhs); }

    template <typename T, typename Alloc>
    inline bool operator<(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) { return stl::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend()); }

    template <typename T, typename Alloc>
    inline bool operator>(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) { return rhs < lhs; }

    template <typename T, typename Alloc>
    inline bool operator<=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) { return !(lhs > rhs); }

    template <typename T, typename Alloc>
    inline bool operator>=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) { return !(lhs < rhs); }
}

#endif // VECTOR_H