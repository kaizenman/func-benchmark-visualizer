#include <benchmark/benchmark.h>

#include <variant>
#include <vector>
#include <random>
#include <sstream>

#include <foo.h>

using argument = std::variant<int, double, std::string>;
std::vector<std::vector<argument>> all_function_calls;
std::vector<std::vector<argument>> n_random_function_calls;

constexpr size_t n_calls = 30;

static void BM_foo(benchmark::State& state)
{
    std::vector<argument>& data = n_random_function_calls[state.range(0)];

    // test
    int a = std::get<int>(data[0]);
    double b = std::get<double>(data[1]);
    int c = std::get<int>(data[2]);

    for (auto _ : state)
    {
        auto result = foo(a, b, c);
        benchmark::DoNotOptimize(result);
    }
}

int main(int arc, char** argv)
{
    std::string function_name = argv[1];
    std::string file_name = function_name + ".txt";
    std::ifstream file(file_name);
    if (!file) {
        std::cout << fmt::format("File {} not found!", file_name) << std::endl;
        return EXIT_FAILURE;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        std::string value;

        std::vector<std::tuple<std::string, std::string>> args;
        while (iss >> type >> value) {
            args.emplace_back(type, value);
        }

        std::vector<argument> function_call;
        for (auto& [type, value] : args) {
            if (type == "string")
                function_call.emplace_back(value);
            else if (type == "int")
                function_call.emplace_back(std::stoi(value));
            else if (type == "double")
                function_call.emplace_back(std::stod(value));
            else
                std::cout << fmt::format("{} type is not implemented!", type) << std::endl;
        }
        all_function_calls.emplace_back(function_call);
    }

    // generate n random function calls
    n_random_function_calls.reserve(n_calls);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, all_function_calls.size() - 1);
    for (size_t i = 0; i < n_calls; ++i) {
        n_random_function_calls.emplace_back(all_function_calls[dis(gen)]);
    }

    // working only for BM_foo right now
    for (size_t i = 0ULL; i < n_calls; ++i) {
        benchmark::RegisterBenchmark(fmt::format("BM_{}", function_name).c_str(), BM_foo)->Args(
            {static_cast<long>(i)}
        );
    }

    benchmark::Initialize(&arc, argv);
    benchmark::RunSpecifiedBenchmarks();
}