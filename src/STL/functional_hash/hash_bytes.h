#ifndef __HASH_BYTES_H__
#define __HASH_BYTES_H__

#include "../traits/type_traits.h"

namespace stl
{
    // Hash function implementation for the nontrivial specialization.
    size_t hash_bytes(const void* ptr, size_t length, size_t seed);
}

#endif // HASH_BYTES_H