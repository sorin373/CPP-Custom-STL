namespace stl
{
    template <typename T, typename Allocator>
    forward_list<T, Allocator>::~forward_list()
    {
        Node* ptr = this->m_head;

        while (ptr != nullptr)
        {
            Node* temp = ptr;
            ptr = ptr->m_next;
            this->m_destroy_node(temp);
        }
    }

    template <typename T, typename Allocator>
    forward_list<T, Allocator>& forward_list<T, Allocator>::operator=(const forward_list& other)
    {
        if (this != &other)
        {
            this->m_clear_list();
            this->m_alloc = allocator_traits::select_on_container_copy_construction(other.m_alloc);
            this->m_range_initialize(other.begin(), other.end());

            return *this;
        }
    }

    template <typename T, typename Allocator>
    forward_list<T, Allocator>& forward_list<T, Allocator>::operator=(std::initializer_list<value_type> ilist)
    {
        this->m_clear_list();
        this->m_range_initialize(ilist.begin(), ilist.end());

        return *this;
    }

    template <typename T, typename Allocator>
    void forward_list<T, Allocator>::assign(size_type count, const_reference value)
    {
        this->m_clear_list();
        this->m_default_initialize(count);
    }

    template <typename T, typename Allocator>
    template <typename InputIt, typename>
    void forward_list<T, Allocator>::assign(InputIt first, InputIt last)
    {
        this->m_clear_list();
        this->m_range_initialize(first, last);
    }

    template <typename T, typename Allocator>
    void forward_list<T, Allocator>::assign(std::initializer_list<value_type> ilist)
    {
        this->m_clear_list();
        this->m_range_initialize(ilist.begin(), ilist.end());
    }
    
    
    /// @b Private members

    template <typename T, typename Allocator>
    void forward_list<T, Allocator>::alloc_fallback(Node* ptr)
    {
        node_allocator alloc = this->m_alloc;
        alloc.destroy(ptr);
        alloc.deallocate(ptr, 1);
    }

    template <typename T, typename Allocator>
    typename forward_list<T, Allocator>::Node* forward_list<T, Allocator>::m_get_node()
    {
        node_allocator alloc = this->m_alloc;
        Node* ptr = alloc.allocate(1);
        return ptr;
    }

    template <typename T, typename Allocator>
    template <typename... Args>
    typename forward_list<T, Allocator>::Node* forward_list<T, Allocator>::m_create_node(Args&&... args)
    {
        node_allocator alloc = this->m_alloc;
        Node* node = this->m_get_node();

        try
        {
            alloc.construct(node, stl::forward<Args>(args)...);
        }
        catch(...)
        {
            this->alloc_fallback(node);
            throw;
        }

        return node;
    }

    template <typename T, typename Allocator>
    void forward_list<T, Allocator>::m_destroy_node(Node* ptr)
    {
        node_allocator alloc = this->m_alloc;
        alloc.destroy(ptr);
        alloc.deallocate(ptr, 1);
    }

    template <typename T, typename Allocator>
    template <typename InputIt>
    void forward_list<T, Allocator>::m_range_initialize(InputIt first, InputIt last)
    {
        Node* head = this->m_head;

        for (; first != last; ++first)
        {   
            head->m_next = this->m_create_node(*first);
            head = head->m_next;
        }
    }

    template <typename T, typename Allocator>
    void forward_list<T, Allocator>::m_default_initialize(size_type size, const_reference value)
    {
        Node* head = this->m_head;

        for (; size; (void)--size)
        {
            head->m_next = this->m_create_node(value);
            head = head->m_next;
        }
    }

    template <typename T, typename Allocator>
    void forward_list<T, Allocator>::m_clear_list()
    {
        if (this->m_head != nullptr)
        {
            Node* head = this->m_head;

            while (head != nullptr)
            {
                Node* temp = head;
                head = head->m_next;
                this->m_destroy_node(temp);
            }
        }
    }

    template <typename T, typename Allocator>
    typename forward_list<T, Allocator>::iterator forward_list<T, Allocator>::m_insert_after(const_iterator pos, size_type count, const_reference value)
    {
        if (count == 0)
            return iterator(pos);

        Node* current = const_cast<Node*>(pos->m_node);
        Node* last_node = nullptr;
        
        for (; count > 0; (void)--count)
        {
            Node *new_node = this->m_create_node(value);

            if (last_node == nullptr)
                last_node = new_node;

            new_node->m_next = current->m_next;
            current->m_next = new_node;
        }

        return iterator(last_node);
    }

    template <typename T, typename Allocator>
    template <typename InputIt, typename>
    typename forward_list<T, Allocator>::iterator forward_list<T, Allocator>::m_insert_after(const_iterator pos, InputIt first, InputIt last)
    {
        if (first == last)
            return iterator(pos);

        Node* current = const_cast<Node*>(pos->m_node);
        Node* last_node = nullptr;

        for (; first != last; ++first)
        {
            Node* new_node = this->m_create_node(*first);

            if (last_node == nullptr)
                last_node = new_node;

            new_node->m_next = current->m_next;
            current->m_next = new_node;
        }

        return iterator(last_node);
    }

    template <typename T, typename Allocator>
    template <typename... Args>
    typename forward_list<T, Allocator>::iterator forward_list<T, Allocator>::m_insert_after(const_iterator pos, Args&&... args)
    {
        Node* current = const_cast<Node*>(pos->m_node);
        Node* new_node = this->m_create_node(stl::forward<Args>(args)...);

        new_node->m_next = current->m_next;
        current->m_next = new_node;

        return iterator(current->m_next);
    }

    template <typename T, typename Allocator>
    typename forward_list<T, Allocator>::iterator forward_list<T, Allocator>::m_erase_after(const_iterator pos)
    {
        Node* current = const_cast<Node*>(pos->m_node), *temp = current->m_next;
        current->m_next = current->m_next->m_next;
        this->m_destroy_node(temp);

        return iterator(current->m_next);
    }

    template <typename T, typename Allocator>
    typename forward_list<T, Allocator>::iterator forward_list<T, Allocator>::m_erase_after(const_iterator first, const_iterator last)
    {
        Node* current = const_cast<Node*>(first->m_node)->m_next;
        
        while (current != last)
        {
            Node* temp = current;
            current = current->m_next;
            
            this->m_destroy_node(temp);
        }

        first->m_next = last;

        return iterator(last);
    }
}