#include <iostream>
#include "vector/vector.h"

int main()
{
    stl::vector<int> v;

    std::cout << v.empty() << std::endl;

    for (int i = 1; i <= 3; i++)
    {
        int x;
        std::cin >> x;
        v.pushback(x);
    }

    std::cout << v.size() << std::endl;

    std::cout << v.capcacity() << std::endl; 

    std::cout << v.max_size() << std::endl;

    std::cout << v.empty() << std::endl;

    // for (size_t i = 0, n = v.size(); i < n; i++)
    //     std::cout << v[i] << " ";

    for (stl::vector<int>::iterator it = v.begin(); it != v.end(); it++)
        std::cout << *it << " ";

    // std::cout << v[4];

    return EXIT_SUCCESS;
}