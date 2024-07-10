#include <iostream>

#include "list/forward_list.h"

#include "vector/vector.h"
#include <vector>

#include <forward_list>

int main()
{
    stl::forward_list<int> t;

    //t.assign(10, 10);

    for (stl::forward_list<int>::iterator it = t.begin(); it != t.end(); it++)
        std::cout << *it << std::endl;

    return EXIT_SUCCESS;
}