#include <iostream>
#include "vector/vector.h"

int main()
{
    // stl::vector<int> v;

    // std::vector<int> vec;

    // std::cout << v.empty() << std::endl;

    // for (int i = 1; i <= 3; i++)
    // {
    //     int x;
    //     std::cin >> x;
    //     v.pushback(x);
    // }

    // std::cout << v.size() << std::endl;

    // std::cout << v.capcacity() << std::endl; 

    // std::cout << v.max_size() << std::endl;

    // std::cout << v.empty() << std::endl;

    // v.resize(2);

    // std::cout << v.size() << std::endl;

    // // for (size_t i = 0, n = v.size(); i < n; i++)
    // //     std::cout << v[i] << " ";

    // for (stl::vector<int>::iterator it = v.begin(); it != v.end(); it++)
    //     std::cout << *it << " ";

    // std::cout << std::endl;

    // v.shrink_to_fit();

    // // for (size_t i = 0; i < 3; i++)
    // //     std::cout << v[i] << " ";

    // // std::cout << v[4];

    stl::vector<int> container;

    container.pushback(1);
    container.pushback(2);
    container.pushback(3);
    container.pushback(4);
    container.pushback(5);
    container.pushback(5);
    container.pushback(5);
    container.pushback(5);
    container.pushback(5);
    container.pushback(5);
    container.pushback(5);
    container.pushback(5);
    container.pushback(5);

    // for (stl::vector<int>::iterator it = container.begin(); it != container.end(); it++)
    // {
    //     std::cout << *it << " ";
    // }

    // std::cout << std::endl;

    // std::cout << container.capcacity() << std::endl;

    // container.resize(3);

    // for (stl::vector<int>::iterator it = container.begin(); it != container.end(); it++)
    // {
    //     std::cout << *it << " ";
    // }

    // std::cout << std::endl;

    // std::cout << container.size() << std::endl;

    // std::cout << container.capcacity() << std::endl;

    // for (size_t i = 0; i < 5; i++)
    //     std::cout << container[i] << " ";

    // container.shrink_to_fit();

    // std::cout << std::endl;

    // for (size_t i = 0; i < 3; i++)
    //     std::cout << container[i] << " ";

    std::cout << container.capcacity() << std::endl;

    container.reserve(20);

    std::cout << container.capcacity() << std::endl;

    for (stl::vector<int>::const_reverse_iterator cit = container.crbegin(); cit != container.crend(); cit++)
    {
        std::cout << *cit << " ";
    }

    std::cout << std::endl;

    std::cout << container.at(2) << std::endl;

    std::cout << container.front() << std::endl;

    std::cout << container.back() << std::endl;

    int *pos = container.data();

    std::cout << *++pos;

    std::cout << container[1000] << std::endl;

    return EXIT_SUCCESS;
}