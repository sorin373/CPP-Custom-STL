#ifndef __REVERSE_ITERATOR_H__
#define __REVERSE_ITERATOR_H__ 1

namespace stl
{
    template <typename T> class reverse_iterator
    {
        typedef T*  iterator;
        typedef T&  reference;

    public:
        reverse_iterator(iterator curr) : current(curr) { }

        iterator operator->() { return current; }

        reference operator*() { return *current; }

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

    template <typename T> class const_reverse_iterator
    {
        typedef const T*  const_iterator;
        typedef const T&  const_reference;

    public:
        const_reverse_iterator(const_iterator curr) : current(curr) { }   

        const_iterator operator->() const { return current; }

        const_reference operator*() const { return *current; }

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

#endif