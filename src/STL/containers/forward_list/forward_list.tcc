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

    template <typename T, typename Allocator>
    template <typename Compare>
    void forward_list<T, Allocator>::merge(forward_list&& other, Compare comp)
    {
        Node* head = this->m_head;
        Node* oth_head = other.m_head;
        Node* new_head = this->m_create_node();
        Node* ptr = new_head;

        while (head->m_next && oth_head->m_next)
        {
            if (comp(*oth_head->m_next->get_m_data(), *head->m_next->get_m_data()))
            {
                ptr->m_next = other.m_head->m_next;
                oth_head = oth_head->m_next;
            }
            else
            {
                ptr->m_next = head->m_next;
                head = head->m_next;
            }

            ptr = ptr->m_next;
        }

        if (oth_head->m_next)
            ptr->m_next = oth_head->m_next;

        if (head->m_next)
            ptr->m_next = head->m_next;

        this->m_head->m_next = new_head->m_next;
        other.m_head->m_next = nullptr;

        this->m_destroy_node(new_head);
    }

    template <typename T, typename Allocator>
    void forward_list<T, Allocator>::splice_after(const_iterator pos, forward_list&&, const_iterator it) noexcept
    {
        const_iterator __it = it;
        ++__it;

        if (pos == it || pos == __it)
            return;

        Node* current = const_cast<Node*>(pos.m_node);
        this->m_insert_after(pos, 1, it.m_node);
    }
    
    template <typename T, typename Allocator>
    typename forward_list<T, Allocator>::size_type forward_list<T, Allocator>::remove(const_reference value)
    {
        size_type count = 0;

        Node* current = this->m_head;
        Node* extra = nullptr;
        
        while (Node* node = static_cast<Node*>(current->m_next))
        {
            if (*node->get_m_data() == value)
            {
                if (node->get_m_data() != stl::addressof(value))
                {
                    this->m_erase_after(current);
                    ++count;
                    continue;
                }
                else
                    extra = node;
            }

            current = current->m_next;
        }

        if (extra != nullptr)
        {
            this->m_erase_after(extra);
            ++count;
        }

        return count;
    }

    template <typename T, typename Allocator>
    template <typename UnaryPredicate>
    typename forward_list<T, Allocator>::size_type forward_list<T, Allocator>::remove_if(UnaryPredicate pred)
    {
        Node* head = this->m_head;
        size_type count = 0;

        while (Node* node = head->m_next)
        {
            if (pred(*node->get_m_data()))
            {
                this->m_erase_after(head);
                ++count;
            }
            else
                head = head->m_next;
        }

        return count;
    }

    template <typename T, typename Allocator>
    template <typename BinaryPredicate>
    typename forward_list<T, Allocator>::size_type forward_list<T, Allocator>::unique(BinaryPredicate pred)
    {
        size_type count = 0;

        iterator first = this->begin();
        iterator last = this->end();

        if (first == last)
            return 0;

        iterator next = first;
        while (++next != last)
        {
            if (pred(*first, *next))
            {
                ++count;
                this->erase_after(first);
            }
            else
                first = next;

            next = first;
        }

        return count;
    }

    template <typename T, typename Allocator>
    template <typename Compare>
    void forward_list<T, Allocator>::sort(Compare comp)
    {
        Node* head = this->m_head->m_next;
        
        if (head == nullptr)
            return;

        bool is_sorted = true;

        do
        {
            is_sorted = true;
            Node* list = head;

            while (list->m_next != nullptr)
            {
                value_type* lhs = list->get_m_data(), *rhs = list->m_next->get_m_data();

                if (!comp(*lhs, *rhs))
                {
                    stl::swap(*lhs, *rhs);
                    is_sorted = false;
                }

                list = list->m_next;
            }
        } while (!is_sorted);
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

        Node* current = const_cast<Node*>(pos.m_node);
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

        Node* current = const_cast<Node*>(pos.m_node);
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
        Node* current = const_cast<Node*>(pos.m_node);
        Node* new_node = this->m_create_node(stl::forward<Args>(args)...);

        new_node->m_next = current->m_next;
        current->m_next = new_node;

        return iterator(current->m_next);
    }

    template <typename T, typename Allocator>
    typename forward_list<T, Allocator>::iterator forward_list<T, Allocator>::m_erase_after(Node* pos)
    {
        if (!pos || !pos->m_next)
            return iterator(nullptr);

        Node *temp = pos->m_next;
        pos->m_next = temp->m_next;
        this->m_destroy_node(temp);

        return iterator(pos->m_next);
    }

    template <typename T, typename Allocator>
    typename forward_list<T, Allocator>::iterator forward_list<T, Allocator>::m_erase_after(const_iterator first, const_iterator last)
    {
        Node* current = const_cast<Node*>(first.m_node)->m_next;
        
        while (current != last)
        {
            Node* temp = current;
            current = current->m_next;
            
            this->m_destroy_node(temp);
        }

        const_cast<Node*>(first.m_node)->m_next = last.m_node;

        return iterator(last.m_node);
    }

    template <typename T, typename Allocator>
    void forward_list<T, Allocator>::m_resize(size_type count, const_reference value)
    {
        iterator it = this->before_begin();
        size_type len = 0;
        
        while (it.m_next() != this->end() && len < count)
        {
            ++it;
            ++len;
        }

        if (len == count)
            this->m_erase_after(it, this->end());
        else
            this->m_insert_after(it, count - len, value);
    }

    template <typename T, typename Allocator>
    typename forward_list<T, Allocator>::iterator forward_list<T, Allocator>::m_splice_after(const_iterator pos, const_iterator first, const_iterator last)
    {
        Node* other = const_cast<Node*>(first.m_node);
        Node* end = other;

        while (end && end->m_next != last.m_node)
            end = end->m_next;

        if (end != other)
            return iterator(this->m_insert_after(pos, other, end));
        
        return iterator(pos.m_node);
    }

    template <typename T, typename Allocator>
    void forward_list<T, Allocator>::m_reverse_after() noexcept
    {
        Node* tail = this->m_head->m_next;

        if (tail == nullptr)
            return;

        while (Node* temp = tail->m_next)
        {
            Node* keep = this->m_head->m_next;

            this->m_head->m_next = temp;
            tail->m_next = temp->m_next;
            temp->m_next = keep;
        }
    }

    template <typename T, typename Allocator>
    bool operator==(const forward_list<T, Allocator>& lhs, const forward_list<T, Allocator>& rhs)
    {
        fwd_list_const_iterator<T> lit = lhs.cbegin(), rit = rhs.cbegin();

        while (lit != lhs.cend() && rit != rhs.cend())
        {
            if (*lit != *rit)
                return false;

            ++lit;
            ++rit;
        }

        if (lit != lhs.end() || rit != rhs.end())
            return false;

        return true;
    }
}