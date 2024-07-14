#ifndef __FORWARD_LIST_H__
#define __FORWARD_LIST_H__ 1

#include "../memory/memory.h"

#include <initializer_list>

namespace stl
{
    template <typename T> class forward_list
    {
        typedef size_t            size_type;
        typedef T                 value_type;
        typedef const value_type* pointer;
        typedef const value_type& reference;

    public:
        class Node
        {
        public:
            Node *m_next;

            Node() : m_data(nullptr), m_next(nullptr)    { }

            Node(value_type data) : m_data(data), m_next(nullptr) { }

            value_type get_m_data() const noexcept  { return m_data; }

            pointer get_m_data_p() const noexcept   { return &m_data; }

            void set_m_data(const T value) noexcept { this->m_data = value; }

        private:
            value_type m_data;
        };

    private:
        typedef Node* node_iterator;
    
    public:
        class iterator
        {
        private:
            Node *m_ptr;

        public:
            iterator(Node *ptr = nullptr) : m_ptr(ptr) { }

            Node *get_node() const noexcept { return m_ptr; }

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

            bool operator!=(const iterator &other) const { return this->m_ptr != other.m_ptr; }

            bool operator==(const iterator &other) const { return this->m_ptr == other.m_ptr; }

            reference operator*() const noexcept { return *this->m_ptr->get_m_data_p(); }

            pointer operator->() const noexcept  { return this->m_ptr->get_m_data(); }
        };

        class const_iterator
        {
        private:
            const Node *m_ptr;

        public:
            const_iterator(const Node *ptr = nullptr) : m_ptr(ptr) { }

            Node *get_node() const noexcept { return m_ptr; }

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

            bool operator!=(const const_iterator &other) const { return this->m_ptr != other.m_ptr; }

            bool operator==(const const_iterator &other) const { return this->m_ptr == other.m_ptr; }

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

        void assign(iterator begin, iterator end)
        {
            if (begin == nullptr) return;

            node_iterator it_1 = begin.get_node();
            node_iterator it_2 = end.get_node();

            while (it_1 != it_2)
            {
                push_front(it_1->get_m_data());

                it_1 = it_1->m_next;
            }
        }

        void assign(const std::initializer_list<T> init)
        {
            if (init.size() <= 0)
                return;

            for (pointer p = init.begin(); p != init.end(); p++)
                push_front(*p);
        }

    private:
        Node *m_head;
    };
}

#endif