#include "hash_bytes.h"

namespace stl 
{
	size_t hash_bytes(const void* ptr, size_t length, size_t seed)
    {
        const unsigned char *data = static_cast<const unsigned char*>(ptr);
        size_t hash = seed;

        for (size_t i = 0; i < length; ++i)
        {
            hash ^= data[i];
            hash *= 0x100000001b3; // 1099511628211
        }

        return hash;
    }
}

/**
 * XOR bit operation 
 * 
   BIT A |  BIT B |   A XOR B
   0	 |  0	  |   0
   0	 |  1	  |   1
   1	 |  0	  |   1
   1	 |  1	  |   0
 */