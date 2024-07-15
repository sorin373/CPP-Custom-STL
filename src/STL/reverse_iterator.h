#ifndef __REVERSE_ITERATOR_H__
#define __REVERSE_ITERATOR_H__ 1

/**
 * @file reverse_iterators.h
 * @brief This header file contains the generic templates for reverse iterators.  
 */

namespace stl
{
    /**
     * @brief This is a template for reverse iterators. It helps to go through a block of memory in reverse without changing the order of the elements.
     * @example for (reverse_iterator<int> it = test.rbegin(); it != test.rend(); it++)
     * @param T Type of element
     */
    template <typename T> class reverse_iterator
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