#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include <new>
#include <limits>

namespace stl
{
	template <typename T>
	class allocator
	{
	public:
		typedef T               value_type;
		typedef T*              pointer;
		typedef const T*        const_pointer;
		typedef T&              reference;
		typedef const T&        const_reference;
		typedef unsigned int    size_type;

		template <typename U>
		struct rebind { typedef allocator<U> other; };

		// pointer address(reference value) const { return &value; }
		// const_pointer address(const_reference value) const { return &value; }

		// allocator() noexcept { }

		// allocator(const allocator& other) noexcept { }

		// template <typename U>
		// allocator(const allocator<U>& other) noexcept { }

		~allocator() noexcept { }

		size_type max_size() const noexcept { return std::numeric_limits<size_type>::max() / sizeof(value_type); }

		pointer allocate(size_type size, const void* hint = nullptr) 
		{
			if (size == 0)
				return nullptr;

			pointer ptr = static_cast<pointer>(::operator new(size * sizeof(value_type)));

			if (!ptr) throw std::bad_alloc();

			return ptr;
		}

		void deallocate(pointer ptr, size_type size) { ::operator delete(ptr); }

		// void construct(pointer ptr, const_reference value) { new(static_cast<void*>(ptr)) T(value); }

		// void destroy(pointer ptr) { ptr->~T(); }
	};

	template <class T1, class T2>
	bool operator==(const allocator<T1>&, const allocator<T2>&) throw() { return true; }

	template <class T1, class T2>
	bool operator!=(const allocator<T1>&, const allocator<T2>&) throw() { return false; }
}

#endif // ALLOCATOR_H