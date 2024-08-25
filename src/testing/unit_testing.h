#ifndef __UNIT_TESTING_H__
#define __UNIT_TESTING_H__

#include "all.h"

#define __TEST_ALL__    0
#define __TEST_ARRAY__  0
#define __TEST_VECTOR__ 1

static void test_array()
{
    constexpr unsigned int __ARRAY_SIZE__ = 4;

    std::cout << "\n+-------------------------------+\n"
              << "| Testing the Array Container   |\n"
              << "+-------------------------------+\n\n";

    array_container_test<int, __ARRAY_SIZE__> array_container_int({1, 2, 3, 4});
    array_container_int.__TEST__();    

    array_container_test<char, __ARRAY_SIZE__> array_container_char({'a', 'b', 'c', 'd'});
    array_container_char.__TEST__();

    array_container_test<double, __ARRAY_SIZE__> array_container_double({1.222, 3.2, 5.00001, 7.1});
    array_container_double.__TEST__();

    array_container_test<std::string, __ARRAY_SIZE__> array_container_string({
        "home", 
        "mateo", 
        "andrei", 
        "cosbuc"
    });
    array_container_string.__TEST__();
    
    struct node 
    {
        bool flag;
        int  value;

        bool operator==(const node& other) const 
        {
            return flag == other.flag && value == other.value;
        }

        bool operator!=(const node& other) const 
        {
            return !(*this == other);
        }
    };

    array_container_test<node, __ARRAY_SIZE__> array_container_struct({
        {true, 1},
        {false, 10},
        {true, 4},
        {false, 10000000}
    });
    array_container_struct.__TEST__();

    array_container_test<long long unsigned int, __ARRAY_SIZE__> array_container_long_long({
        100000000000, 
        200000000000, 
        300000000000, 
        400000000000
    });
    array_container_long_long.__TEST__();
}

static void test_vector()
{
    std::cout << "\n+-------------------------------+\n";
    std::cout << "| Testing the Vector Container  |\n";
    std::cout << "+-------------------------------+\n\n";

    vector_container_test<int> vector_container_int = {1, 2, 3, 4};
    vector_container_int.__TEST__();
}

void INIT_UNIT_TESTS()
{
#if __TEST_ARRAY__ || __TEST_ALL__
    test_array();
#endif

#if __TEST_VECTOR__ || __TEST_ALL__
    test_vector();
#endif
}

#endif // UNIT_TESTING_H