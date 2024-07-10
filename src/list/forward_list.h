#ifndef __FORWARD_LIST_H__
#define __FORWARD_LIST_H__ 1

#include "../memory/memory.h"

namespace stl
{
    template <typename T> class forward_list
    {
        typedef size_t   size_type;
        typedef T        value_type;
        typedef const T* pointer;
        typedef const T& reference;

    public:
        class Node
        {
        public:
            Node *m_next;

            Node() : m_data(nullptr), m_next(nullptr)    { }

            Node(T data) : m_data(data), m_next(nullptr) { }

            T get_m_data() const noexcept           { return m_data; }

            pointer get_m_data_p() const noexcept   { return &m_data; }

            void set_m_data(const T value) noexcept { this->m_data = value; }

        private:
            value_type m_data;
        };

        class iterator
        {
        private:
            Node *m_ptr;

        public:
            iterator(Node *ptr = nullptr) : m_ptr(ptr) { }

            iterator& operator++()
            {
                if (m_ptr != nullptr)
                    m_ptr = m_ptr->m_next;

                return *this;
            }

            iterator operator++(int) 
            {
                iterator temp(*this);

                m_ptr = m_ptr->m_next;

                return temp;
            }

            bool operator!=(const iterator &other) const { return m_ptr != other.m_ptr; }

            bool operator==(const iterator &other) const { return m_ptr == other.m_ptr; }

            reference operator*() const noexcept { return *this->m_ptr->get_m_data_p(); }

            pointer operator->() const noexcept  { return this->m_ptr->get_m_data(); }
        };

        class const_iterator
        {
        private:
            const Node *m_ptr;

        public:
            const_iterator(const Node *ptr = nullptr) : m_ptr(ptr) { }

            const_iterator& operator++()
            {
                if (m_ptr != nullptr)
                    m_ptr = m_ptr->m_next;

                return *this;
            }

            const_iterator operator++(int)
            {
                const_iterator temp(*this);

                m_ptr = m_ptr->m_next;

                return temp;
            }

            bool operator!=(const const_iterator &other) const { return m_ptr != other.m_ptr; }

            bool operator==(const const_iterator &other) const { return m_ptr == other.m_ptr; }

            reference operator*() const noexcept { return *this->m_ptr->get_m_data_p(); }

            pointer operator->() const noexcept  { return this->m_ptr->get_m_data(); }
        };

    public:
        forward_list() noexcept : m_head(nullptr) { }

        iterator begin() const noexcept        { return m_head; }

        const_iterator cbegin() const noexcept { return m_head; }

        iterator end() const noexcept          { return nullptr; }

        T front() const noexcept               { return m_head->get_m_data(); }

        void push_front(T value)
        {
            Node *node = (Node *)malloc(sizeof(Node));

            if (node == nullptr)
                throw std::runtime_error("malloc: couldn't allocate memory!\n");

            node->set_m_data(value);
            node->m_next = nullptr;

            if (this->m_head == nullptr)
                this->m_head = node;
            else
            {
                node->m_next = this->m_head;
                this->m_head = node;
            }
        }

        void assign(size_type size, value_type value)
        {
            while (size > 0)
            {
                push_front(value);
                size--;
            }
        }

        void assign()
        {

        }

        void assign()
        {

        }

    private:
        Node *m_head;
    };
}

#endif