#include "testing/test.h"

#define DEBUG_ALL 1

int main()
{
#if DEBUG_ALL
    Test::init();
#endif

    return 0;
}