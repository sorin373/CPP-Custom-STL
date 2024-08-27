#include "testing/unit_testing.h"

#include <iostream>
#include <chrono>
#include <cstdlib>
#include <stdio.h>

static int exit_code = 0;

static void on_exit() { std::cout << "Exit code: " << exit_code << "\n"; }

int main()
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    
    INIT_UNIT_TESTS();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    
    long long diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

    std::cout << "-------------------------\n" 
              << "Process exited in " << diff / 1000.00 << 's' << std::endl;
    
    atexit(&on_exit);

    return exit_code;
}