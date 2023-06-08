#include <benchmark/benchmark.h>

int main(int arc, char** argv)
{
    benchmark::Initialize(&arc, argv);
    benchmark::RunSpecifiedBenchmarks();
}