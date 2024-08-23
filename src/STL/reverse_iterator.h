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

    @file reverse_iterators.h

    @brief This is part of the standard template libraries (STL) and it is used to implement reverse iterators
 */

#ifndef __REVERSE_ITERATOR_H__
#define __REVERSE_ITERATOR_H__

namespace stl
{
    /**
     * @brief This is a template for reverse iterators. It helps to go through a block of memory in reverse without changing the order of the elements.
     * @example for (reverse_iterator<int> it = test.rbegin(); it != test.rend(); it++)
     * @param T Type of element
     */
    template <typename T> 
    class reverse_iterator
    {
        typedef T*  iterator;
        typedef T&  reference;

    public:
        reverse_iterator() = default;

        constexpr explicit reverse_iterator(iterator curr) : current(curr) { }

        constexpr iterator operator->() { return current - 1; }

        constexpr reference operator*() { return *(current - 1); }

        reverse_iterator& operator++()
        {
            current--;
            return *this;
        }

        reverse_iterator operator++(int)
        {
            reverse_iterator temp = *this;

            current--;

            return temp;
        }

        bool operator==(const reverse_iterator &other) const { return this->current == other.current; }

        bool operator!=(const reverse_iterator &other) const { return this->current != other.current; }

    private:
        iterator current;
    };

    /**
     * @brief This is a template for constant reverse iterators. 
     *        It helps to go through a block of memory in reverse without modifying the elements or changing their order.
     * @example for (const_reverse_iterator<int> it = test.crbegin(); it != test.crend(); it++)
     * @param T Type of element
     */
    template <typename T> class const_reverse_iterator
    {
        typedef const T*  const_iterator;
        typedef const T&  const_reference;

    public:
        const_reverse_iterator() = default;

        constexpr explicit const_reverse_iterator(const_iterator curr) : current(curr) { }   

        constexpr const_iterator operator->() const { return current - 1; }

        constexpr const_reference operator*() const { return *(current - 1); }

        const_reverse_iterator& operator++()
        {
            current--;
            return *this;
        }

        const_reverse_iterator operator++(int)
        {
            const_reverse_iterator temp = *this;

            current--;

            return temp;
        }

        bool operator==(const const_reverse_iterator &other) { return this->current == other.current; }

        bool operator!=(const const_reverse_iterator &ohter) { return this->current != ohter.current; }

    private:
        const_iterator current;
    };
}

#endif // REVERSE_ITERATOR_H