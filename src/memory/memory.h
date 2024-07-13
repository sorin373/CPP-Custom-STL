#ifndef MEMORY_H
#define MEMORY_H

typedef unsigned int        size_t;
typedef unsigned char       uint8_t;
typedef unsigned long long  uint64_t;

/**
 * @brief Copies the values of size bytes from src to dest.
*/
void memcpy(void *dest, const void *src, size_t size);

#endif // MEMORY_H