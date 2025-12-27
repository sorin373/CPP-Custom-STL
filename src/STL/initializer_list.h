#ifndef __INITIALIZER_LIST__
#define __INITIALIZER_LIST__

#include "core.h"

namespace stl
{
    template <typename T>
    class initializer_list
    {
    public:
        typedef T             value_type;
        typedef stl::size_t   size_type;

        typedef T&            reference;
        typedef const T&      const_reference;
        
        typedef T*            iterator;
        typedef const T*      const_iterator;

        constexpr initializer_list() noexcept : m_array(0), m_size(0) { }

        constexpr initializer_list(const_iterator it, size_type size) : m_array(it), m_size(size) { }

        constexpr size_type size() const noexcept { return m_size; }

        constexpr const_iterator begin() const noexcept { return m_array; }

        constexpr const_iterator end() const noexcept { return m_array + m_size; }

    private:
        const_iterator  m_array;
        size_type       m_size;
    };

    template <typename T, stl::size_t N>
    initializer_list<T> to_initializer_list(const T (&array)[N]) { return initializer_list<T>(array, N); }
}

#endif // INITIALIZER_LIST