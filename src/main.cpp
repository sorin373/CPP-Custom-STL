#include <iostream>

#include "vector/vector.h"

#include <vector>

class test
{
private:
    int t;

public:
    test(int x) : t(x) 
    { 
       
    };

    ~test() = default;
};

int main()
{
    stl::vector<int> v{1, 2, 3, 4, 5, 6};

                    // 0  1  2  3  4  5
    
    stl::vector<test> t;

    v.emplace_back(2);

    //v.swap(t);

    for (size_t i = 0; i < v.size(); i++)
        std::cout << v[i] << " ";

    std::cout << "\n";

    // for (size_t i = 0; i < t.size(); i++)
    //     std::cout << t[i] << " ";

    std::cout << "\n";

    for (stl::vector<int>::iterator it = v.begin(); it != v.end(); it++)
        if (*it % 2 == 0)
        {
            v.erase(it);
            it--;
        }

    // for (size_t i = 0; i < v.size(); i++)
    //     std::cout << v[i] << " ";

    return EXIT_SUCCESS;
}