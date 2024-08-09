#include "testing/test.h"

#define DEBUG_ALL 1

void run_tests();

int main()
{
#if DEBUG_ALL
    run_tests();
#endif

    return 0;
}

void run_tests() { Test::init(); }