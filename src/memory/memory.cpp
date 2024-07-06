#include "memory.h"

void memcpy(void *dest, const void *src, size_t size)
{
    uint8_t *csrc = (uint8_t*)src, *cdest = (uint8_t*)dest;

    for (size_t i = 0; i < size; i++)
        cdest[i] = csrc[i];
}