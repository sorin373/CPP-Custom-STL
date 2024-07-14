#include <iostream>

#include "STL/array.h"

int main()
{
    stl::array<int, 5> test = {1, 2, 3, 4, 5};

    for (stl::const_reverse_iterator<int> it = test.crbegin(); it != test.crend(); it++)
        std::cout << *it << " ";
    
    return EXIT_SUCCESS;
}