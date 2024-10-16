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

    @file  reverse_iterators.h

    @brief This is part of the standard template libraries (STL) and it is an iterator adaptor that reverses the direction of a given iterator
 */

#pragma once

#include "traits/type_traits.h"

namespace stl
{
    // Base tag for input iterators
    struct input_iterator_tag {};

    // Base tag for output iterators
    struct output_iterator_tag {};

    // Forward iterator tag, derived from input_iterator_tag
    struct forward_iterator_tag : public input_iterator_tag {};

    // Bidirectional iterator tag, derived from forward_iterator_tag
    struct bidirectional_iterator_tag : public forward_iterator_tag {};

    // Random access iterator tag, derived from bidirectional_iterator_tag
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};
    
    /// @brief Primary template (fallback) | SFINAE Compatibility
    template <typename Iterator, typename = __void_t<>>
    struct __iterator_traits { };

    /**
     * @brief  Specialization template for @c __iterator_traits
     * @tparam Iterator Type of iterator
     * @note   It assumes that the iterator type (`e.g., std::vector<int>::iterator`) defines these types as nested types
     * 
     * @typedef @c `difference_type`    -  type that represents the difference between two iterators
     * @typedef @c `value_type`         -  type of the elements pointed to by the iterator
     * @typedef @c `pointer`            -  type of a pointer to an element pointed to by the iterator
     * @typedef @c `reference`          -  type of a reference to an element pointed to by the iterator
     * @typedef @c `iterator_category`  -  category of the iterator `[input / output / forward / bidirectional / random access]`
     */
    template <typename Iterator>
    struct __iterator_traits<Iterator, __void_t<
        typename Iterator::difference_type,
        typename Iterator::value_type,
        typename Iterator::pointer,
        typename Iterator::reference,
        typename Iterator::iterator_category>>
    {
        typedef typename Iterator::difference_type     difference_type;
        typedef typename Iterator::value_type          value_type;
        typedef typename Iterator::pointer             pointer;
        typedef typename Iterator::reference           reference;
        typedef typename Iterator::iterator_category   iterator_category;
    };

    /** 
     * @brief  A simple wrapper around @c __iterator_traits with which the user directly interacts with
     * @tparam Iterator Type of iterator
     */ 
    template <typename Iterator>
    struct iterator_traits : public __iterator_traits<Iterator> { };
    
    /// @brief Partial specialization for raw pointers (e.g., int*)
    template <typename T>
    struct iterator_traits<T*>
    {
        typedef stl::ptrdiff_t                         difference_type;
        typedef T                                      value_type;
        typedef T*                                     pointer;
        typedef T&                                     reference;
        typedef random_access_iterator_tag             iterator_category;
    };

    /// @brief Partial specialization for const raw pointers (e.g., const int*)
    template <typename T>
    struct iterator_traits<const T*>
    {
        typedef stl::ptrdiff_t                         difference_type;
        typedef T                                      value_type;
        typedef const T*                               pointer;
        typedef const T&                               reference;
        typedef random_access_iterator_tag             iterator_category;
    };

    /**
     * @brief An iterator adaptor that reverses the direction of a given iterator
     * @tparam T Type of iterator to adapt
     */
    template <typename T>
    class reverse_iterator
    {
    public: 
        typedef T                                                           iterator_type;
        typedef typename stl::iterator_traits<T>::iterator_category         iterator_category;
        typedef typename stl::iterator_traits<T>::value_type                value_type;
        typedef typename stl::iterator_traits<T>::difference_type           difference_type;
        typedef typename stl::iterator_traits<T>::pointer                   pointer;
        typedef typename stl::iterator_traits<T>::reference                 reference;

        constexpr reverse_iterator() : m_current() { }

        constexpr explicit reverse_iterator(iterator_type other) : m_current(other) { }

        template <typename U>
        constexpr reverse_iterator(const reverse_iterator<U>& other) : m_current(other.m_current) { }

        template <typename U>
        constexpr reverse_iterator& operator=(const reverse_iterator<U>& other)
        {
            this->m_current = other.m_current;
            return *this;
        }

        constexpr iterator_type base() const noexcept { return m_current; }

        constexpr reference operator*() const
        {
            iterator_type temp = m_current;
            return *(--temp);
        }

        constexpr pointer operator->() const
        {
            iterator_type temp = m_current;
            --temp;
            return &(*temp);
        }

        constexpr reference operator[](difference_type n) const
        {
            return *(*this + n);
        }

        constexpr reverse_iterator& operator++()
        {
            --m_current;
            return *this;
        }

        constexpr reverse_iterator operator++(int)
        {
            reverse_iterator temp = *this;
            --m_current;
            return temp;
        }

        constexpr reverse_iterator& operator--()
        {
            ++m_current;
            return *this;
        }

        constexpr reverse_iterator operator--(int)
        {
            reverse_iterator temp = *this;
            ++m_current;
            return temp;
        }

        constexpr reverse_iterator operator+(difference_type n) const { return reverse_iterator(m_current - n); }

        constexpr reverse_iterator operator-(difference_type n) const { return reverse_iterator(m_current + n); }

        constexpr reverse_iterator& operator+=(difference_type n)
        {
            m_current -= n;
            return *this;
        }

        constexpr reverse_iterator& operator-=(difference_type n)
        {
            m_current += n;
            return *this;
        }

        template <typename U>
        constexpr bool operator==(const reverse_iterator<U>& other) const { return m_current == other.m_current; }

        template <typename U>
        constexpr bool operator!=(const reverse_iterator<U>& other) const { return m_current != other.m_current; }

    private:
        iterator_type m_current;
    };

    template <typename Iterator>
    using RequireIterator = typename enable_if<is_convertible<typename iterator_traits<Iterator>::iterator_category, input_iterator_tag>::value>::type;

    template <typename Iterator>
    constexpr typename iterator_traits<Iterator>::difference_type distance(Iterator first, Iterator last)
    {
        using category = typename iterator_traits<Iterator>::iterator_category;
        static_assert(is_base_of<input_iterator_tag, category>::value);

        if (is_base_of<random_access_iterator_tag, category>::value)
            return last - first;
        else
        {
            typename iterator_traits<Iterator>::difference_type result = 0;

            while (first != last)
            {
                ++first;
                ++result;
            }

            return result;
        }
    }

    namespace __detail
    {
        template <typename It>
        void do_advance(It& __it, typename stl::iterator_traits<It>::difference_type __n, stl::input_iterator_tag)
        {
            while (__n > 0)
            {
                --__n;
                ++__it;
            }
        }

        template <typename It>
        void do_advance(It& __it, typename stl::iterator_traits<It>::difference_type __n, stl::bidirectional_iterator_tag)
        {
            while (__n > 0)
            {
                --__n;
                ++__it;
            }

            while (__n < 0)
            {
                ++__n;
                --__it;
            }
        }

        template <typename It>
        void do_advance(It& __it, typename stl::iterator_traits<It>::difference_type __n, stl::random_access_iterator_tag)
        { __it += __n; }
    }

    template <typename It, typename Distance>
    void advance(It& it, Distance n)
    { __detail::do_advance(it, typename stl::iterator_traits<It>::difference_type(n), typename stl::iterator_traits<It>::iterator_category()); }

    template <typename InputIt>
    constexpr InputIt next(InputIt it, typename stl::iterator_traits<InputIt>::difference_type n = 1)
    {
        stl::advance(it, n);
        return it;
    }
}