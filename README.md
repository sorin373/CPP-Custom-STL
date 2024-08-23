# CPP-Custom-STL

- [array.h](https://github.com/sorin373/CPP-Custom-STL/blob/main/src/STL/array.h)

Here’s a brief example demonstrating how to use `stl::array`:
```C++
#include "array.h"  // Include the header file for stl::array
#include <iostream>

int main() 
{
    // Create an array of 100 integers
    stl::array<int, 100> my_array;

    // Fill the entire array with the value 10
    my_array.fill(10);

    // Output the first element of the array
    std::cout << my_array[0] << std::endl;

    return 0;
}
```

- [vector.h](https://github.com/sorin373/CPP-Custom-STL/blob/main/src/STL/vector.h)
- [forward_list.h](https://github.com/sorin373/CPP-Custom-STL/blob/main/src/STL/forward_list.h)
- [unordered_map.h](https://github.com/sorin373/CPP-Custom-STL/blob/main/src/STL/unordered_map.h)

## Custom generic implemetations
- [reverse_iterator.h](https://github.com/sorin373/CPP-Custom-STL/blob/main/src/STL/reverse_iterator.h)
- [allocator.h](https://github.com/sorin373/CPP-Custom-STL/blob/main/src/STL/allocator.h)

More documentation when all STL components are finished :)