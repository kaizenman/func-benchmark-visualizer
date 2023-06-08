#pragma once

#include <profiler/profiler.h>

int foo(int a, double b, int c)
{
    profiler::record_function(a, b, c);

    return a + b + c;
}

