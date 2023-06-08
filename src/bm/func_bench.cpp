#include <benchmark/benchmark.h>

#include <variant>
#include <vector>
#include <random>
#include <sstream>
#include <any>

#include <foo.h>

#include <bm/any_argument_adapter.h>

using arg_type = std::variant<int, double, std::string>;

std::vector<std::vector<arg_type>> all_function_calls;

static std::unordered_map<std::string, std::function<arg_type(std::vector<arg_type>)>> adapted_functions = {
    {"foo", adaptfunc(foo)},
    {"bar", adaptfunc(bar)}
};

static std::unordered_map<std::string, std::vector<std::vector<arg_type>>> function_arguments = {
};

constexpr size_t n_calls = 30;
std::string function_name;

static void BM_fn(benchmark::State& state)
{
    std::function<arg_type(std::vector<arg_type>)> fn = adapted_functions[function_name];
    std::vector<arg_type>& args = function_arguments[function_name][state.range(0)];
    
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(fn(args));
        benchmark::ClobberMemory();
    }
}

int main(int arc, char** argv)
{
    function_name = argv[1];
    
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

        std::vector<arg_type> function_call;
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
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, all_function_calls.size() - 1);
    for (size_t i = 0; i < n_calls; ++i) {
        function_arguments[function_name].emplace_back(all_function_calls[dis(gen)]);
    }

    for (size_t i = 0ULL; i < n_calls; ++i) {
        benchmark::RegisterBenchmark(fmt::format("BM_{}", function_name).c_str(), BM_fn)->Args(
            {static_cast<long>(i)}
        );
    }

    benchmark::Initialize(&arc, argv);
    benchmark::RunSpecifiedBenchmarks();
}