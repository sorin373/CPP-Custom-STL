#include <iostream>

#include "vector/vector.h"

int main()
{
    stl::vector<int> v;

    v.assign({1, 2, 3});

    for (stl::vector<int>::iterator it = v.begin(); it != v.end(); it++)
        std::cout << *it << " ";    

    return EXIT_SUCCESS;
}