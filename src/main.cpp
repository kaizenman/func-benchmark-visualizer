#include <iostream>

#include <profiler/profiler.h>

#define ENABLE_PROFILING

void foo(int a, int b, int c)
{
    profiler::record_function(a, b, c);
}

void bar(int a, double b, std::string c)
{
    profiler::record_function(a, b, c);
}

int main() 
{
    foo(1, 2, 3);
    foo(2, 3, 4);
    foo(3, 4, 5);

    bar(1, 2.0, "3");
    bar(2, 3.0, "4");
    bar(3, 4.0, "5");

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
