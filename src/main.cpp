#include "testing/array_test.h"

#include <iostream>
#include <chrono>
#include <cstdlib>

#include <string>

int exit_code = 0;

static void on_exit() { std::cout << "Exit code: " << exit_code << "\n"; }

int main()
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    const int N_INT = 4;
    array_container_test<int, N_INT> ac_int({1, 2, 3, 4});
    ac_int.__TEST__();    

    const int N_CHAR = 4;
    array_container_test<char, N_CHAR> ac_char({'a', 'b', 'c', 'd'});
    ac_char.__TEST__();

    const int N_DOUBLE = 4;
    array_container_test<double, N_DOUBLE> ac_double({1.222, 3.2, 5.00001, 7.1});
    ac_double.__TEST__();

    const int N_STRING = 4;
    array_container_test<std::string, N_STRING> ac_string({"sorin", "mateo", "andrei", "cosbuc"});
    ac_string.__TEST__();
    
    struct node 
    {
        bool flag;
        int value;

        bool operator==(const node& other) const 
        {
            return flag == other.flag && value == other.value;
        }

        bool operator!=(const node& other) const 
        {
            return !(*this == other);
        }
    };

    const int N_STRUCT = 4;
    array_container_test<node, N_STRUCT> ac_struct({
        {true, 1},
        {false, 10},
        {true, 4},
        {false, 10000000}
    });
    ac_struct.__TEST__();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    long long diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

    std::cout << "\nProcess exited in " << diff / 1000.00 << 's' << std::endl;
    
    atexit(&on_exit);

    return exit_code;
}