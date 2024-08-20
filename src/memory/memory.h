#ifndef __MEMORY_H__
#define __MEMORY_H__

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

/** @brief Copies the values of size bytes from src to dest. */
void memcpy(void *dest, const void *src, unsigned int size);

#endif // MEMORY_H