#include <iostream>

#include "forward_list.h"

#include "array.h"

int main()
{
    stl::forward_list<int> t;

    stl::forward_list<int> x;

    x.assign(10, 10);

    t.assign(x.begin(), x.end());

    for (stl::forward_list<int>::iterator it = t.begin(); it != t.end(); it++)
        std::cout << *it << std::endl;

    stl::array<int, 5> test;

    test.at(19);

    return EXIT_SUCCESS;
}