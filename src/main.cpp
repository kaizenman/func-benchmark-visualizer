#include <iostream>

#define ENABLE_PROFILING
#include "foo.h"

int main() 
{
    for (int i = 0; i < 100; ++i) {
        foo(1, 2.0, 3);
        foo(4, 5.0, 6);
        foo(7, 8.0, 9);

        bar(1, 2.0, 3);
        bar(4, 5.0, 6);
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
