#pragma once

#include "../traits/type_traits.h"
#include "hash_bytes.h"

namespace stl
{
    template <
        typename Result, 
        typename Arg
    > struct hash_base
    {
        typedef Result  result_type;
        typedef Arg     argument_type;
    };
    
    // Primary class template hash.
    template <typename Key>
    struct hash;

    // Partial specializations for pointer types.
    template <typename T>
    struct hash<T*> : public hash_base<size_t, T*>
    {
        size_t operator()(T *ptr) const noexcept
        {
            return reinterpret_cast<size_t>(ptr);
        }
    };

    // Explicit specializations for integer types.
    #define cxx_hashtable_define_trivial_hash(T)  \
    template <>                                   \
    struct hash<T> : public hash_base<size_t, T>  \
    {                                             \
        size_t operator()(T value) const noexcept \
        {                                         \
            return static_cast<size_t>(value);    \
        }                                         \
    };

    cxx_hashtable_define_trivial_hash(bool)
    cxx_hashtable_define_trivial_hash(char)
    cxx_hashtable_define_trivial_hash(signed char)
    cxx_hashtable_define_trivial_hash(unsigned char)
    cxx_hashtable_define_trivial_hash(wchar_t)
    cxx_hashtable_define_trivial_hash(char16_t)
    cxx_hashtable_define_trivial_hash(char32_t)
    cxx_hashtable_define_trivial_hash(short)
    cxx_hashtable_define_trivial_hash(int)
    cxx_hashtable_define_trivial_hash(long)
    cxx_hashtable_define_trivial_hash(long long)
    cxx_hashtable_define_trivial_hash(unsigned short)
    cxx_hashtable_define_trivial_hash(unsigned int)
    cxx_hashtable_define_trivial_hash(unsigned long)
    cxx_hashtable_define_trivial_hash(unsigned long long)

    #undef cxx_hashtable_define_trivial_hash

    struct hash_impl
    {
        static size_t hash(const void* ptr, size_t length, size_t seed = static_cast<size_t>(0xc70f6907UL))
        {
            return hash_bytes(ptr, length, seed);
        }

        template <typename T>
        static size_t hash(const T& value)
        {
            return hash(&value, sizeof(value));
        }

        template <typename T>
        static size_t hash_combine(const T& value, size_t hash)
        {
            return hash(&value, sizeof(value), hash);
        }
    };

    #define cxx_hashtable_define_trivial_hash(T)            \
    template <>                                             \
    struct hash<T> : public hash_base<size_t, T>            \
    {                                                       \
        size_t operator()(T value) const noexcept           \
        {                                                   \
            return value != 0 ? hash_impl::hash(value) : 0; \
        }                                                   \
    };

    // Specialization for float.
    cxx_hashtable_define_trivial_hash(float)

    // Specialization for double.
    cxx_hashtable_define_trivial_hash(double)

    #undef cxx_hashtable_define_trivial_hash

    // Specialization for long double.
    template <>
    struct hash<long double> : public hash_base<size_t, long double>
    {
        size_t operator()(long double value) const noexcept
        {
            uint64_t int_representation = *reinterpret_cast<const uint64_t*>(&value);
            return int_representation ^ (int_representation >> 32);
        }
    };

    inline size_t stl_hash_string(const char* str)
    {
        unsigned long hash_value = 0;
        
        for (; *str; ++str)
            hash_value = 5 * hash_value + *str;

        return static_cast<size_t>(hash_value);
    }

    #define cxx_hashtable_define_trivial_hash(T)  \
    template <>                                   \
    struct hash<T> : public hash_base<size_t, T>  \
    {                                             \
        size_t operator()(T value) const noexcept \
        {                                         \
            return stl_hash_string(value);        \
        }                                         \
    };

    cxx_hashtable_define_trivial_hash(char*)
    cxx_hashtable_define_trivial_hash(const char*)

    #undef cxx_hashtable_define_trivial_hash
}