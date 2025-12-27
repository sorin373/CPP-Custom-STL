#pragma once

#include "../../allocator/allocator.h"
#include "../../iterator.h"

namespace stl
{
    template <typename T>
    struct list_node
    {
        list_node() 
            : m_storage(), m_next(nullptr), m_prev(nullptr) { }

        list_node(T data) 
            : m_storage(data), m_next(nullptr), m_prev(nullptr) { }

        T* m_data() noexcept { return &this->m_storage; }

        const T* m_data() const noexcept { return &this->m_storage; }

        list_node<T> *m_next, *m_prev;

    private:
        T m_storage;
    };

    template <typename T>
    struct list_iterator
    {
        typedef list_iterator<T>                 Self;
        typedef list_node<T>                     Node;
        typedef stl::ptrdiff_t                   difference_type;
        typedef stl::bidirectional_iterator_tag  iterator_category;
        typedef T                                value_type;
        typedef T*                               pointer;
        typedef T&                               reference;

        list_iterator() noexcept 
            : m_node() { }

        explicit list_iterator(Node *__n) noexcept
            : m_node(__n) { }

        reference operator*() const noexcept
        { return *this->m_node->m_data(); }

        pointer operator->() const noexcept
        { return this->m_node->m_data(); }

        Self& operator++() noexcept
        {
            this->m_node = this->m_node->m_next;
            return *this;
        }

        Self operator++(int) noexcept
        {
            Self temp = *this;
            this->m_node = this->m_node->m_next;
            return temp;
        }

        Self& operator--() noexcept
        {
            this->m_node = this->m_node->m_prev;
            return *this;
        }

        Self operator--(int) noexcept
        {
            Self temp = *this;
            this->m_node = this->m_node->m_prev;
            return *this;
        }

        bool operator==(const Self& __n) const noexcept
        { return this->m_node == __n.m_node; }

        bool operator!=(const Self& __n) const noexcept
        { return this->m_node != __n.m_node; }

        Node *m_node;
    };

    template <typename T>
    struct const_list_iterator
    {
        typedef const_list_iterator<T>           Self;
        typedef const list_node<T>               Node;
        typedef list_iterator<T>                 iterator;
        typedef stl::ptrdiff_t                   difference_type;
        typedef stl::bidirectional_iterator_tag  iterator_category;
        typedef T                                value_type;
        typedef const T*                         pointer;
        typedef const T&                         reference;

        const_list_iterator() noexcept
            : m_node() { }

        explicit const_list_iterator(Node *__n)
            : m_node(__n) { }

        const_list_iterator(const iterator& __it) noexcept
            : m_node(__it.m_node) { }

        reference operator*() const noexcept
        { return *this->m_node->m_storage; }

        pointer operator->() const noexcept
        { return this->m_node->m_storage; }

        Self& operator++() noexcept
        {
            this->m_node = this->m_node->m_next;
            return *this;
        }

        Self operator++(int) noexcept
        {
            Self temp = *this;
            this->m_node = this->m_node->m_next;
            return *this;
        }

        Self& operator--() noexcept
        {
            this->m_node = this->m_node->m_prev;
            return *this;
        }

        Self operator--(int) noexcept
        {
            Self temp = *this;
            this->m_node = this->m_node->m_prev;
            return *this;
        }

        bool operator==(const Self& __n) const noexcept
        { return this->m_node == __n.m_node; }

        bool operator!=(const Self& __n) const noexcept
        { return this->m_node != __n.m_node; }

        Node *m_node;
    };

    template <
        typename T, 
        typename Allocator = stl::allocator<T>
    > class list
    {
        using node_allocator = typename Allocator::template rebind<list_node<T>>::other;

    public:
        typedef T                                                         value_type;
        typedef Allocator                                                 allocator_type;
        typedef stl::size_t                                               size_type;
        typedef stl::ptrdiff_t                                            difference_type;
        typedef value_type&                                               reference;
        typedef const value_type&                                         const_reference;
        typedef typename stl::allocator_traits<Allocator>::pointer        pointer;
        typedef typename stl::allocator_traits<Allocator>::const_pointer  const_pointer;
        typedef list_node<T>                                              Node;

        list() noexcept
            : m_head(nullptr), m_tail(nullptr) 
        { this->m_head = this->m_tail = this->m_create_node(); }

        explicit list(const Allocator& alloc)
            : m_alloc(alloc), m_head(nullptr), m_tail(nullptr) 
        { this->m_head = this->m_tail = this->m_create_node(); }

        list(size_type count, const_reference value, const Allocator& alloc = Allocator())
            : m_head(nullptr), m_tail(nullptr), m_alloc(alloc)
        {
            this->m_head = this->m_tail = this->m_create_node();
            this->m_default_initialize(count, value);
        }

        explicit list(size_type count)
            : list(count) { }

        explicit list(size_type count, const Allocator& alloc = Allocator())
            : m_head(nullptr), m_tail(nullptr), m_alloc(alloc)
        {
            this->m_head = this->m_tail = this->m_create_node();
            this->m_default_initialize(count);
        }

        template <typename InputIt, typename = stl::RequireIterator<InputIt>>
        list(InputIt first, InputIt last, const Allocator& alloc = Allocator())
            : m_head(nullptr), m_tail(nullptr), m_alloc(alloc)
        {
            this->m_head = this->m_tail = this->m_create_node();
            this->m_range_initialize(first, last);
        }

        list(const list& other)
            : m_head(nullptr), m_tail(nullptr), m_alloc(other.m_alloc) 
        {
            Node *n_head = this->m_head, o_head = other.m_head; 
            
            while (o_head != nullptr)
            {
                Node *node = this->m_create_node(o_head->m_data());

                n_head->m_next = node;
                node->m_prev = n_head;
                n_head = n_head->m_next;

                o_head = o_head->m_next;
            }
        }

        list(list&& other) noexcept
            : m_head(other.m_head), m_tail(other.m_tail), m_alloc(other.m_alloc) 
        { other.m_head = other.m_tail = nullptr; }






        void print()
        {
            Node *head = this->m_head->m_next;

            while (head != nullptr)
            {
                std::cout << *head->m_data() << " ";
                head = head->m_next;
            }
        }



    private:
        list_node<T>  *m_head, *m_tail;
        allocator_type m_alloc;

        void alloc_fallback(Node *ptr);

        template <typename... Args>
        Node* m_create_node(Args&&... args);

        void m_default_initialize(size_type size, const_reference value = value_type());

        template <typename InputIt>
        void m_range_initialize(InputIt first, InputIt last);
    };
}

#include "list.tcc"