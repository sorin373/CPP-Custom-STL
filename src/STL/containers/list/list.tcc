namespace stl
{
    template <typename T, typename Allocator>
    void list<T, Allocator>::alloc_fallback(Node *ptr)
    {
        node_allocator alloc = this->m_alloc;
        alloc.destroy(ptr);
        alloc.deallocate(ptr, 1);
    }

    template <typename T, typename Allocator>
    template <typename... Args>
    typename list<T, Allocator>::Node* list<T, Allocator>::m_create_node(Args&&... args)
    {   
        node_allocator alloc = this->m_alloc;
        Node *node = alloc.allocate(1);

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
    void list<T, Allocator>::m_default_initialize(size_type size, const_reference value)
    {
        Node *head = this->m_head;

        for (; size; (void)--size)
        {
            Node *node = this->m_create_node(value);

            head->m_next = node;
            node->m_prev = head;
            head = head->m_next;
        }
    }

    template <typename T, typename Allocator>
    template <typename InputIt>
    void list<T, Allocator>::m_range_initialize(InputIt first, InputIt last)
    {
        Node* head = this->m_head;

        for (; first != last; ++first)
        {
            Node *node = this->m_create_node(*first);

            head->m_next = node;
            node->m_prev = head;
            head = head->m_next;
        }
    }
}