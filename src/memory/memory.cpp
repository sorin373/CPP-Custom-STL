#include "memory.h"

void memcpy(void *dest, const void *src, unsigned int size)
{
    const unsigned char *csrc = static_cast<const unsigned char*>(src);
    unsigned char *cdest = static_cast<unsigned char*>(dest);

    for (unsigned int i = 0; i < size; ++i)
        cdest[i] = csrc[i];
}