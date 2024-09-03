#ifndef __UNIT_TESTING_H__
#define __UNIT_TESTING_H__

#include "all.h"

#include <complex>

#define INT_MAX		2147483647
#define INT_MIN		(-INT_MAX-1)

#define __TEST_ALL__             0 
#define __TEST_ARRAY__           0 
#define __TEST_VECTOR__          0

#define __TEST_FUNCTIONAL_HASH__ 0

class node 
{
private:
    bool flag;
    int  value;

public:
    node() { }
    node(bool f, int v) : flag(f), value(v) { }

    bool operator==(const node& other) const { return flag == other.flag && value == other.value; }
    bool operator!=(const node& other) const { return !(*this == other); }
};

static void test_array()
{
    constexpr unsigned int __ARRAY_SIZE__ = 4;

    std::cout << "\n+-------------------------------+\n"
              << "| Testing the Array Container   |\n"
              << "+-------------------------------+\n\n";

    array_container_test<int, __ARRAY_SIZE__> array_container_int({
        INT_MIN, 
        2147483000, 
        2147483646, 
        INT_MAX
    });
    array_container_int.__TEST__();    

    array_container_test<char, __ARRAY_SIZE__> array_container_char({'a', 'b', 'c', 'd'});
    array_container_char.__TEST__();

    array_container_test<double, __ARRAY_SIZE__> array_container_double({1.222, 3.2, 5.00001, 7.1});
    array_container_double.__TEST__();

    array_container_test<std::string, __ARRAY_SIZE__> array_container_string({
        "a set of words that is complete in itself, typically containing a subject and predicate", 
        "the punishment assigned to a defendant found guilty by a court", 
        "The term 'sentence' is widely used to refer to quite different types of unit. Grammatically, it is the highest unit and consists of one independent clause, or two or more related clauses. Orthographically and rhetorically, it is that unit which starts with a capital letter and ends with a full stop, question mark or exclamation mark.", 
        "lorem"
    });
    array_container_string.__TEST__();

    array_container_test<node, __ARRAY_SIZE__> array_container_struct({
        {true,  2147480000},
        {false, 2147483000},
        {true,  2147483646},
        {false, 2147483647}
    });
    array_container_struct.__TEST__();

    array_container_test<long long unsigned int, __ARRAY_SIZE__> array_container_long_long({
        100000000000, 
        200000000000, 
        300000000000, 
        400000000000
    });
    array_container_long_long.__TEST__();

    array_container_test<bool, __ARRAY_SIZE__> array_container_bool({true, false, true, false});
    array_container_bool.__TEST__();

    array_container_test<stl::array<int, __ARRAY_SIZE__>, __ARRAY_SIZE__> array_container_array({
        {INT_MIN, -1, 0, 1},
        {100, 1000, 10000, 100000},
        {1000000, 2000000, 3000000, 4000000},
        {2147483644, 2147483645, 2147483646, INT_MAX}
    });
    array_container_array.__TEST__();

    array_container_test<short, 0> array_container_null({});
    array_container_null.__TEST__();

    array_container_test<std::complex<double>, __ARRAY_SIZE__> array_container_complex({
        {1.0, 2.0},
        {3.0, 4.0},
        {5.0, 6.0},
        {7.0, 8.0}
    });

    enum class Color { Red, Green, Blue, Yellow };

    array_container_test<Color, __ARRAY_SIZE__> array_container_enum({
        Color::Red,
        Color::Green,
        Color::Blue,
        Color::Yellow
    });
    array_container_enum.__TEST__();
}

static void test_vector()
{
    std::cout << "\n+-------------------------------+\n";
    std::cout << "| Testing the Vector Container  |\n";
    std::cout << "+-------------------------------+\n\n";

    std::cout << "=== INT ===\n\n";

    vector_container_test<int> vector_container_int = {
        INT_MIN, 
        2147483000, 
        2147483646, 
        INT_MAX
    };
    vector_container_int.__TEST__();

    std::cout << "=== DOUBLE ===\n\n";

    vector_container_test<double> vector_container_double = {
        1.7976931348623157E+308, 
        1.5976931348623157E+308, 
        1.3976931348623157E+308, 
        1.1976931348623157E+308
    };
    vector_container_double.__TEST__();

    std::cout << "=== CHAR ===\n\n";

    vector_container_test<char> vector_container_char = {'a', 'b', 'c', 'd'};
    vector_container_char.__TEST__();

    std::cout << "=== BOOL ===\n\n";

    vector_container_test<bool> vector_container_bool = {true, false, true, false};
    vector_container_bool.__TEST__();

    std::cout << "=== LONG LONG UNSIGNED INT ===\n\n";

    vector_container_test<long long unsigned int> vector_container_long_long({
        100000000000, 
        200000000000, 
        300000000000, 
        400000000000
    });
    vector_container_long_long.__TEST__();

    std::cout << "=== COMPLEX | std::complex<double> ===\n\n";

    vector_container_test<std::complex<double>> vector_container_complex({
        {1.0, 2.0},
        {3.0, 4.0},
        {5.0, 6.0},
        {7.0, 8.0}
    });
    vector_container_complex.__TEST__();

    std::cout << "=== STRUCT ===\n\n";
    vector_container_test<node> vector_container_struct({
        {true,  2147480000},
        {false, 2147483000},
        {true,  2147483646},
        {false, 2147483647},
        {true,  INT_MAX},
        {false, INT_MAX},
        {true,  INT_MIN},
        {false, INT_MIN}
    });
    vector_container_struct.__TEST__();

    std::cout << "=== 2D VECTOR ===\n\n";

    vector_container_test<stl::vector<int>> vector_container_vector({
        {INT_MIN, -1, 0, 1},
        {100, 1000, 10000, 100000},
        {1000000, 2000000, 3000000, 4000000},
        {2147483644, 2147483645, 2147483646, INT_MAX}
    });
    vector_container_vector.__TEST__();
}

void INIT_UNIT_TESTS()
{
    traits_test::__TEST__();

#if __TEST_FUNCTIONAL_HASH__
    test_hash_naive();
#endif

#if __TEST_ARRAY__ || __TEST_ALL__
    test_array();
#endif

#if __TEST_VECTOR__ || __TEST_ALL__
    test_vector();
#endif
}

#endif // UNIT_TESTING_H