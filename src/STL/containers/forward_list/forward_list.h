#ifndef __FORWARD_LIST_H__
#define __FORWARD_LIST_H__

#include "../../allocator/allocator.h"
#include "../../iterator.h"
#include "../../traits/allocator_traits.h"
#include "../../../cUtility/stl_function.h"
#include "../../algorithm/algorithm.h"

#include <initializer_list>

namespace stl
{
    template <typename T>
    struct fwd_list_node
    {
        fwd_list_node() : m_storage(), m_next(nullptr) { }

        fwd_list_node(T data) : m_storage(data), m_next(nullptr) { }

        T* get_m_data() noexcept { return &this->m_storage; } 
        
        const T* get_m_data() const noexcept { return &this->m_storage; }

        fwd_list_node<T>* m_next;

    private:
        T m_storage;
    };

    template <typename T>
    struct fwd_list_iterator
    {
        typedef fwd_list_iterator<T>            Self;
        typedef fwd_list_node<T>                Node;

        typedef T                               value_type;
        typedef T*                              pointer;
        typedef T&                              reference;
        typedef stl::ptrdiff_t                  difference_type;
        typedef stl::forward_iterator_tag       iterator_category;

        fwd_list_iterator() noexcept : m_node() { }

        explicit fwd_list_iterator(Node* __n) noexcept : m_node(__n) { }

        reference operator*() const noexcept { return *static_cast<Node*>(this->m_node)->get_m_data(); }

        pointer operator->() const noexcept { return static_cast<Node*>(this->m_node)->get_m_data(); }

        Self& operator++() noexcept
        {
            this->m_node = this->m_node->m_next;
            return *this;
        }

        Self operator++(int) noexcept
        {
            Self temp(*this);
            this->m_node = this->m_node->m_next;
            return temp;
        }

        bool operator==(const Self& __x) const noexcept { return this->m_node == __x.m_node; }

        bool operator!=(const Self& __x) const noexcept { return this->m_node != __x.m_node; }

        Self m_next() const noexcept
        {
            if (this->m_node)
                return fwd_list_iterator(this->m_node->m_next);

            return fwd_list_iterator(nullptr);
        }
        
        Node* m_node;
    };

    template <typename T>
    struct fwd_list_const_iterator
    {
        typedef fwd_list_const_iterator<T>      Self;
        typedef const fwd_list_node<T>          Node;

        typedef T                               value_type;
        typedef const T*                        pointer;
        typedef const T&                        reference;
        typedef stl::ptrdiff_t                  difference_type;
        typedef stl::forward_iterator_tag       iterator_category;

        fwd_list_const_iterator() noexcept : m_node(nullptr) { }

        explicit fwd_list_const_iterator(Node* __n) noexcept : m_node(__n) { }

        fwd_list_const_iterator(const fwd_list_iterator<T>& __iter) noexcept : m_node(__iter.m_node) { }

        reference operator*() const noexcept { return *static_cast<Node*>(this->m_node)->get_m_data(); }

        pointer operator->() const noexcept { return static_cast<Node*>(this->m_node)->get_m_data(); }

        Self& operator++() noexcept
        {
            this->m_node = this->m_node->m_next;
            return *this;
        }

        Self operator++(int) noexcept
        {
            Self temp(*this);
            this->m_node = this->m_node->m_next;
            return temp;
        }

        bool operator==(const Self& __x) const noexcept { return this->m_node == __x.m_node; }

        bool operator!=(const Self& __x) const noexcept { return this->m_node != __x.m_node; }

        Self m_next() const noexcept
        {
            if (this->m_node)
                return fwd_list_const_iterator(this->m_node->m_next);

            return fwd_list_const_iterator(nullptr);
        }

        Node* m_node;
    };

    /**
     * @brief std::forward_list is a container that supports fast insertion and removal of elements from anywhere in the container.
     *        It is implemented as a singly-linked list. 
     * @tparam T The type of the elements @tparam Allocator An allocator that is used to acquire/release memory and to construct/destroy the elements in that memory.
     */
    template <
        typename T,
        typename Allocator = stl::allocator<T>
    > class forward_list
    {
        /***************
        * MEMBER TYPES *
        ***************/

        using allocator_traits = stl::allocator_traits<Allocator>;
        using node_allocator = typename Allocator::template rebind<fwd_list_node<T>>::other;

    public:
        typedef T                                      value_type;
        typedef Allocator                              allocator_type;
        typedef stl::size_t                            size_type;
        typedef stl::ptrdiff_t                         difference_type;
        typedef typename Allocator::reference          reference;
        typedef typename Allocator::const_reference    const_reference;
        typedef typename Allocator::pointer            pointer;
        typedef typename Allocator::const_pointer      const_pointer;
        typedef fwd_list_iterator<T>                   iterator;
        typedef fwd_list_const_iterator<T>             const_iterator;
        typedef fwd_list_node<T>                       Node;

        /**********************************
        * MEMBER FUNCTIONS - CONSTRUCTORS *
        **********************************/

        forward_list() noexcept
            : m_head(nullptr) 
        { this->m_head = this->m_create_node(); }

        forward_list(const Allocator& alloc) 
            : m_alloc(alloc), m_head(nullptr) 
        { this->m_head = this->m_create_node(); }

        forward_list(size_type count, const_reference value, const Allocator& alloc = Allocator())
            :m_head(nullptr), m_alloc(alloc) 
        { 
            this->m_head = this->m_create_node();
            this->m_default_initialize(count, value); 
        }

        explicit forward_list(size_type count)
            : m_head(nullptr) 
        { 
            this->m_head = this->m_create_node();
            this->m_default_initialize(count); 
        }

        explicit forward_list(size_type count, const Allocator& alloc = Allocator())
            : m_head(nullptr), m_alloc(alloc) 
        {
            this->m_head = this->m_create_node(); 
            this->m_default_initialize(count); 
        }

        template <typename InputIt, typename = stl::RequireIterator<InputIt>>
        forward_list(InputIt first, InputIt last, const Allocator& alloc = Allocator())
            : m_head(nullptr), m_alloc(alloc) 
        {
            this->m_head = this->m_create_node();
            this->m_range_initialize(first, last); 
        }

        forward_list(const forward_list& other)
            : m_head(nullptr), m_alloc(allocator_traits::select_on_container_copy_construction(other.m_alloc)) 
        {
            this->m_head = this->m_create_node(); 
            this->m_range_initialize(other.cbegin(), other.cend()); 
        }

        forward_list(const forward_list& other, const Allocator& alloc)
            : m_head(nullptr), m_alloc(alloc) 
        {
            this->m_head = this->m_create_node();
            this->m_range_initialize(other.cbegin(), other.cend()); 
        }

        forward_list(std::initializer_list<value_type> ilist, const Allocator& alloc = Allocator()) 
            : m_head(nullptr), m_alloc(alloc)
        {
            this->m_head = this->m_create_node(); 
            this->m_range_initialize(ilist.begin(), ilist.end()); 
        }

        /*************
        * DESTRUCTOR *
        *************/

        ~forward_list();

        forward_list& operator=(const forward_list& other);

        forward_list& operator=(std::initializer_list<value_type> ilist);

        void assign(size_type count, const_reference value);

        template <typename InputIt, typename = stl::RequireIterator<InputIt>>
        void assign(InputIt first, InputIt last);

        void assign(std::initializer_list<value_type> ilist);

        allocator_type get_allocator() const noexcept 
        { return this->m_alloc; }

        /*****************
        * ELEMENT ACCESS *
        *****************/

        reference front() 
        { return *this->m_head->m_next->get_m_data(); }

        const_reference front() const 
        { return *this->m_head->m_next->get_m_data(); }

        /************
        * ITERATORS *
        ************/

        iterator before_begin() noexcept 
        { return iterator(this->m_head); }

        const_iterator cbefore_begin() const noexcept 
        { return const_iterator(this->m_head); }

        iterator begin() noexcept 
        { return iterator(this->m_head->m_next); }

        const_iterator cbegin() const noexcept 
        { return const_iterator(this->m_head->m_next); }

        iterator end() noexcept 
        { return iterator(0); }
        
        const_iterator cend() const noexcept 
        { return const_iterator(0); }

        /***********
        * CAPACITY *
        ***********/

        bool empty() const noexcept 
        { return this->m_head->m_next == 0; }

        size_type max_size() const noexcept
        { return std::numeric_limits<difference_type>::max(); }

        /************
        * MODIFIERS *
        ************/
 
        void clear() noexcept 
        { this->m_clear_list(); }

        iterator insert_after(const_iterator pos, const_reference value) 
        { return iterator(this->m_insert_after(pos, value)); }

        iterator insert_after(const_iterator pos, size_type count, const_reference value) 
        { return iterator(this->m_insert_after(pos, count, value)); }

        template <typename InputIt, typename = stl::RequireIterator<InputIt>>
        iterator insert_after(const_iterator pos, InputIt first, InputIt last)
        { return iterator(this->m_insert_after(pos, first, last)); }

        iterator insert_after(const_iterator pos, std::initializer_list<value_type> ilist)
        { return iterator(this->m_insert_after(pos, ilist.begin(), ilist.end())); }

        template <typename... Args>
        iterator emplace_after(const_iterator pos, Args&&... args)
        { return iterator(this->m_insert_after(pos, stl::forward<Args>(args)...)); }

        iterator erase_after(const_iterator pos)
        { return iterator(this->m_erase_after(pos)); }

        iterator erase_after(const_iterator first, const_iterator last)
        { return iterator(this->m_erase_after(first, last)); }

        void push_front(const_reference value)
        { this->m_insert_after(this->before_begin(), value); }

        void push_front(T&& value)
        { this->m_insert_after(this->before_begin(), stl::move(value)); }

        template <typename... Args>
        reference emplace_front(Args&&... args)
        { return *this->m_insert_after(this->before_begin(), stl::forward<Args>(args)...); }

        void pop_front() { this->m_erase_after(this->m_head); }

        void resize(size_type count) { this->m_resize(count); };

        void resize(size_type count, const_reference value) { this->m_resize(count, value); };

        void swap(forward_list& other)
        { 
            if (this != &other)
            {
                stl::swap(this->m_head->m_next, other.m_head->m_next);
                stl::swap(this->m_alloc, other.m_alloc);
            }
        }

        template <typename Compare>
        void merge(forward_list&& other, Compare comp);

        void merge(forward_list&& other)
        { this->merge(stl::move(other), stl::less<value_type>()); }

        void merge(forward_list& other)
        { this->merge(stl::move(other)); }

        template <typename Compare>
        void merge(forward_list& other, Compare comp)
        { this->merge(stl::move(other), comp); }

        void splice_after(const_iterator pos, forward_list&& other) noexcept
        {
            if (!other.empty())
                this->m_splice_after(pos, other.before_begin(), other.end());
        }

        void splice_after(const_iterator pos, forward_list& other) noexcept
        { this->splice_after(pos, stl::move(other)); }

        void splice_after(const_iterator pos, forward_list&& other, const_iterator it) noexcept;

        void splice_after(const_iterator pos, forward_list& other, const_iterator it) noexcept
        { this->splice_after(pos, stl::move(other), it); }

        void splice_after(const_iterator pos, forward_list&&, const_iterator first, const_iterator last) noexcept
        { this->m_splice_after(pos, first, last); }

        void splice_after(const_iterator pos, forward_list&, const_iterator first, const_iterator last) noexcept
        { this->m_splice_after(pos, first, last); }
        
        size_type remove(const_reference value);

        template <typename UnaryPredicate>
        size_type remove_if(UnaryPredicate pred);

        void reverse() noexcept 
        { this->m_reverse_after(); }

        template <typename BinaryPredicate>
        size_type unique(BinaryPredicate pred);

        void unique() 
        { this->unique(stl::equal_to<value_type>()); }

        template <typename Compare>
        void sort(Compare comp);

        void sort() 
        { this->sort(stl::less<value_type>()); }

    private:
        Node*            m_head;
        allocator_type   m_alloc;

        void alloc_fallback(Node* ptr);

        Node* m_get_node();

        template <typename... Args>
        Node* m_create_node(Args&&... args);

        void m_destroy_node(Node* ptr);

        template <typename InputIt>
        void m_range_initialize(InputIt first, InputIt last);

        void m_default_initialize(size_type size, const_reference value = value_type());

        void m_clear_list();

        iterator m_insert_after(const_iterator pos, size_type count, const_reference value);

        template <typename InputIt, typename = stl::RequireIterator<InputIt>>
        iterator m_insert_after(const_iterator pos, InputIt first, InputIt last);

        template <typename... Args>
        iterator m_insert_after(const_iterator pos, Args&&... args);

        iterator m_erase_after(Node* pos);

        iterator m_erase_after(const_iterator first, const_iterator last);

        void m_resize(size_type count, const_reference value = value_type());

        iterator m_splice_after(const_iterator pos, const_iterator first, const_iterator last);
    
        void m_reverse_after() noexcept;
    };

    template <typename T, typename Allocator>
    bool operator==(const forward_list<T, Allocator>& lhs, const forward_list<T, Allocator>& rhs);

    template <typename T, typename Allocator>
    inline bool operator<(const forward_list<T, Allocator>& lhs, const forward_list<T, Allocator>& rhs)
    { return stl::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend()); }

    template <typename T, typename Allocator>
    inline bool operator!=(const forward_list<T, Allocator>& lhs, const forward_list<T, Allocator>& rhs)
    { return !(lhs == rhs); }

    template <typename T, typename Allocator>
    inline bool operator>(const forward_list<T, Allocator>& lhs, const forward_list<T, Allocator>& rhs)
    { return rhs < lhs; }

    template <typename T, typename Allocator>
    inline bool operator>=(const forward_list<T, Allocator>& lhs, const forward_list<T, Allocator>& rhs)
    { return !(lhs < rhs); }

    template <typename T, typename Allocator>
    inline bool operator<=(const forward_list<T, Allocator>& lhs, const forward_list<T, Allocator>& rhs)
    { return !(rhs < lhs); }

    template <typename T, typename Allocator>
    inline void swap(forward_list<T, Allocator>& lhs, forward_list<T, Allocator>& rhs) noexcept(noexcept(lhs.swap(rhs)))
    { lhs.swap(rhs); }
}

#include "forward_list.tcc"

#endif // FORWARD_LIST_H