#ifndef __UNIQUE_PTR_H__
#define __UNIQUE_PTR_H__

namespace stl
{
    template <typename T> class unique_ptr
    {
        typedef T value_type;

    public:
        unique_ptr() noexcept : m_ptr(nullptr) { }

        explicit unique_ptr(value_type *ptr) noexcept : m_ptr(ptr) { }

        unique_ptr(const unique_ptr &other) = delete;
        unique_ptr& operator=(const unique_ptr &other) = delete;

        unique_ptr& operator=(unique_ptr &other) noexcept
        {
            if (this != &other)
            {
                delete m_ptr;
                m_ptr = other.m_ptr;
                other.m_ptr = nullptr;
            }

            return *this;
        }

        T* get() const noexcept { return m_ptr; } 

        T* release() noexcept
        {
            T *temp = m_ptr;
            m_ptr = nullptr;
            return temp;
        }

        void reset(T *ptr = nullptr) noexcept
        {
            delete m_ptr;
            m_ptr = ptr;
        }

        T* operator->() const noexcept { return m_ptr; }

        T& operator*() const noexcept  { return *m_ptr; }

        explicit operator bool() const noexcept { return m_ptr != nullptr; }

    private:
        T *m_ptr;
    };
}

#endif // UNIQUE_PTR_H