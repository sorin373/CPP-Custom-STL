#ifndef MEMORY_H
#define MEMORY_H 1

#if _WIN32 || _WIN64
   #if _WIN64
     #define ENV 64
  #else
    #define ENV 32
  #endif
#endif

#if __GNUC__
  #if __x86_64__ || __ppc64__
    #define ENV 64
  #else
    #define ENV 32
  #endif
#endif

typedef unsigned int        size_t;
typedef unsigned char       uint8_t;
typedef unsigned long long  uint64_t;

/**
 * @brief Copies the values of size bytes from src to dest.
*/
void memcpy(void *dest, const void *src, size_t size);

#endif // MEMORY_H