#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "core.h"

static constexpr stl::size_t __META_SIZE_T = sizeof(stl::size_t);

void *memcpy(void *dest, const void *src, stl::size_t size)
{
    unsigned char *uc_dest = static_cast<unsigned char*>(dest);
    const unsigned char *uc_src = static_cast<const unsigned char*>(src);
    
    // Align to the size of a word
    while ((stl::uintptr_t)uc_dest % __META_SIZE_T != 0 && size > 0)
    {
        *uc_dest++ = *uc_src++;
        --size; 
    }

    // Copy words if possible
    stl::size_t *dw = (stl::size_t*)uc_dest;
    const stl::size_t *sw = (const stl::size_t*)uc_src;

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

#endif // MEMORY_H