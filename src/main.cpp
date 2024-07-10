#include <iostream>

#include "list/forward_list.h"

int main()
{
    stl::forward_list<int> t;

    stl::forward_list<int> x;

    x.assign(10, 10);

    t.assign(x.begin(), x.end());

    for (stl::forward_list<int>::iterator it = t.begin(); it != t.end(); it++)
        std::cout << *it << std::endl;

    return EXIT_SUCCESS;
}