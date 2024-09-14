#include <new>
#include <limits>

namespace stl
{
    template <typename T>
    typename allocator<T>::size_type allocator<T>::max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    template <typename T>
    typename allocator<T>::pointer allocator<T>::allocate(size_type size, const void* hint)
    {
        if (size == 0)
            return nullptr;

        pointer ptr = static_cast<pointer>(::operator new(size * sizeof(value_type)));

        if (!ptr) throw std::bad_alloc();

        return ptr;
    }

    template <typename T>
    void allocator<T>::deallocate(pointer ptr, size_type size)
    {
        ::operator delete(ptr);
    }

    template <typename T>
    void allocator<T>::construct(pointer ptr, const_reference value)
    {
        ::new(static_cast<void*>(ptr)) T(value);
    }

    template <typename T>
    template <typename... Args>
    void allocator<T>::construct(pointer ptr, Args&&... args)
    {
        ::new(static_cast<void*>(ptr)) T(stl::forward<Args>(args)...);
    }
}