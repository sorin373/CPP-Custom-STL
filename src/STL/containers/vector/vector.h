#pragma once

#include "../../iterator.h"
#include "../../allocator/allocator.h"
#include "../../algorithm/algorithm.h"

#include <initializer_list>
#include <stdexcept>

#define OUT_OF_BOUNDS_EXCEPTION throw std::out_of_range("Index out of bounds!\n");

namespace stl
{
    /**
     *  @brief %vector (STL) container (a dynamic C-style array). It adds extra functionalites to the basic CPP arrays, by resizing the memory when necessary.
     *         In addition it consists in many member functions that help the user manage the array data faster and more efficiently. 
     *         Subscripting ( @c [] ) access is also provided as with C-style arrays.
     *  @tparam T The type of the elements. @tparam Allocator An allocator that is used to acquire/release memory and to construct/destroy the elements in that memory.
     */
    template <
        typename T, 
        typename Allocator = stl::allocator<T>
    > class vector
    {
        using allocator_traits = stl::allocator_traits<Allocator>;

    public:
        typedef T                 value_type;
        typedef Allocator         allocator_type;
        typedef stl::size_t       size_type;
        typedef stl::ptrdiff_t    difference_type;

        typedef T*                iterator;
        typedef const T*          const_iterator;

        typedef T&                reference;
        typedef const T&          const_reference;
        
        typedef typename stl::reverse_iterator<iterator>              reverse_iterator;
        typedef typename stl::reverse_iterator<const_iterator>  const_reverse_iterator;

        vector() noexcept 
            : m_capacity(0), m_size(0), m_data(nullptr) { }
        
        explicit vector(const allocator_type& alloc) 
            : m_alloc(alloc), m_capacity(0), m_size(0), m_data(nullptr) { }

        explicit vector(size_type count, const value_type& value = value_type(), const Allocator& alloc = Allocator())
            : m_alloc(alloc), m_capacity(count), m_size(count), m_data(nullptr)
        { this->m_default_initialize(count, value); }

        vector(std::initializer_list<value_type> ilist, const Allocator& alloc = Allocator())
            : m_alloc(alloc), m_capacity(ilist.size()), m_size(ilist.size()), m_data(nullptr)
        { this->m_range_initialize(ilist.begin(), ilist.end()); }

        vector(const vector& other) 
            : m_alloc(allocator_traits::select_on_container_copy_construction(other.m_alloc)), m_size(other.m_size), m_capacity(other.m_capacity), m_data(nullptr) 
        { this->m_range_initialize(other.cbegin(), other.cend()); }

        vector(const vector& other, const Allocator& alloc) 
            : m_alloc(alloc), m_size(other.size()), m_capacity(other.capcacity()), m_data(nullptr)
        { this->m_range_initialize(other.cbegin(), other.cend()); }

        ~vector() 
        {
            if (this->m_data != nullptr) 
            {   
                this->clear();
                this->m_alloc.deallocate(this->m_data, this->m_capacity);
                this->m_data = nullptr;
            }
        }

        vector& operator=(const vector& other)
        {
            if (this != &other)
            {
                this->m_alloc = allocator_traits::select_on_container_copy_construction(other.m_alloc);
                this->assign(other.cbegin(), other.cend());
            }

            return *this;
        }

        vector& operator=(const std::initializer_list<T> ilist)
        { this->assign(ilist.begin(), ilist.end()); }

        void assign(size_type size, const_reference value);

        template <typename InputIt, typename = stl::RequireIterator<InputIt>>
        void assign(InputIt first, InputIt last);

        void assign(std::initializer_list<value_type> ilist)
        { this->assign(ilist.begin(), ilist.end()); }

        constexpr allocator_type get_allocator() const noexcept { return m_alloc; }
        
        reference at(size_type pos) 
        { 
            if (pos >= m_size) OUT_OF_BOUNDS_EXCEPTION 
            return *(m_data + pos); 
        }

        const_reference at(size_type pos) const 
        { 
            if (pos >= m_size) OUT_OF_BOUNDS_EXCEPTION 
            return *(m_data + pos);
        }

        reference operator[](size_type pos) { return this->at(pos); }
        constexpr const_reference operator[](size_type pos) const { return this->at(pos); }

        reference front() noexcept { return *this->m_data; }
        constexpr const_reference front() const noexcept { return *this->m_data; }

        reference back() noexcept { return *(this->m_data + this->m_size - 1); }
        constexpr const_reference back() const noexcept { return *(this->m_data + this->m_size - 1); }

        value_type* data() noexcept { return this->m_data; }
        constexpr value_type* data() const noexcept { return this->m_data; }

        iterator begin() noexcept { return iterator(this->m_data); }
        constexpr const_iterator cbegin() const noexcept { return const_iterator(this->m_data); }
        iterator end() noexcept { return iterator(this->m_data + this->m_size); }
        constexpr const_iterator cend() const noexcept { return const_iterator(this->m_data + this->m_size); }

        reverse_iterator rbegin() noexcept { return reverse_iterator(this->m_data + this->m_size); }
        reverse_iterator rend() noexcept { return reverse_iterator(this->m_data); }
        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(this->m_data + this->m_size); }
        const_reverse_iterator crend() const noexcept { return const_reverse_iterator(this->m_data); } 

        constexpr bool empty() const noexcept { return this->m_size == 0; }
        constexpr size_type size() const noexcept { return this->m_size; }
        constexpr size_type max_size() { return std::numeric_limits<difference_type>::max(); }

        /**
         * @brief Increase the capacity of the vector (the total number of elements that the vector can hold without requiring reallocation)
         *        reserve() does not change the size of the vector.
         * @param new_cap new capacity of the vector, in number of elements
         * @throw std::length_error if @c new_cap exceeds @c max_size().
         * @throw Any exception thrown by @fn Allocator::allocate() (typically std::bad_alloc).
         * 
         * @example stl::vector<Type> my_vector;
         * 
         *          my_vector.reserve(10);            // allocates 10 * sizeof(Type)
         *          assert(my_vector.capacity(), 10); // true
         */
        void reserve(size_type new_cap);
        
        constexpr size_type capacity() const noexcept { return this->m_capacity; }

        void shrink_to_fit();

        void clear() noexcept;
        
        iterator insert(const_iterator pos, size_type count, const_reference value);

        iterator insert(const_iterator pos, const_reference value)
        { return this->insert(pos, 1, value); }

        template <typename InputIt, typename = stl::RequireIterator<InputIt>> 
        iterator insert(const_iterator pos, InputIt first, InputIt last);

        iterator insert(const_iterator pos, std::initializer_list<value_type> ilist)
        { return this->insert(pos, ilist.begin(), ilist.end()); }

        void resize(size_type count, const_reference value);
        
        /**
         * @brief This function resizes the container. While the m_size increses by 1 the m_capacity is calculated 
         *        to be >= to m_size so that the container is resized efficently. The formula I came up with is:
         *        m_capacity = new_size + new_size / 2 + 1. It is designed to grow as the new_size gets bigger and bigger,
         *        balancing the need of resizes.
         * @param new_size The new size to which the vector will be resized to
         * @throw If the new memory block can not be allocated, a runtime error will be thrown.
         */
        void resize(size_type count) { this->resize(count, value_type()); }

        void push_back(const_reference element);

        void pop_back();

        void erase(iterator begin, iterator end);

        void erase(iterator pos) { this->erase(pos, pos + 1); }

        void swap(vector& payload) noexcept;

        template <typename... Args> iterator emplace(iterator position, Args&&... args);

        template <typename... Args> iterator emplace_back(Args&&... args);

        iterator find(value_type value);

    private:
        value_type*     m_data;
        size_type       m_size;
        size_type       m_capacity;
        allocator_type  m_alloc;

        void alloc_fallback();

        void m_default_initialize(size_type count, const_reference value = value_type());

        template <typename InputIt>
        void m_range_initialize(InputIt first, InputIt last);
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

#include "vector.tcc"