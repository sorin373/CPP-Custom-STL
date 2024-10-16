#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "traits/type_traits.h"

namespace stl
{
    static constexpr size_t __META_SIZE_T = sizeof(size_t);

    inline void *memcpy(void *dest, const void *src, size_t size)
    {
        unsigned char *uc_dest = static_cast<unsigned char*>(dest);
        const unsigned char *uc_src = static_cast<const unsigned char*>(src);
        
        // Align to the size of a word
        while ((uintptr_t)uc_dest % __META_SIZE_T != 0 && size > 0)
        {
            *uc_dest++ = *uc_src++;
            --size; 
        }

        // Copy words if possible
        size_t *dw = (size_t*)uc_dest;
        const size_t *sw = (const size_t*)uc_src;

        while (size > __META_SIZE_T)
        {
            *dw++ = *sw++;
            size -= __META_SIZE_T;
        }

        // Copy any remaining bytes
        uc_dest = reinterpret_cast<unsigned char*>(dw);
        uc_src = reinterpret_cast<const unsigned char*>(sw);

        while (size--)
            *uc_dest++ = *uc_src++;

        return dest;
    }
}

#endif // MEMORY_H