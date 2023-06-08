#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <mutex>
#include <map>
#include <fmt/format.h>

namespace profiler
{

namespace detail
{

class files_holder final
{
public:
    std::ofstream& getFile(const std::string& name)
    {
        if (!m_files.contains(name)) {
            std::ofstream file(name);
            if (file.is_open())
                m_files.emplace(name, std::move(file));
            else
                std::cerr << "Failed to open file " << name << '\n';
        }
        return m_files[name];
    }

    ~files_holder()
    {
        for (auto& [name, file] : m_files) {
            if (file.is_open())
                file.close();
        }
    }
private:
    std::map<std::string, std::ofstream> m_files;
};

template<typename T>
std::string convert_to_string(const T& value)
{
    if constexpr (std::is_same_v<T, std::string>)
        return fmt::format("string {}", value);
    else if constexpr (std::is_same_v<T, int>)
        return fmt::format("int {}", value);
    else if constexpr (std::is_same_v<T, double>)
        return fmt::format("double {}", value);
    else
        return "not implemented"; // TODO
}


template<typename... Args>
std::string args_to_string(Args&&... arguments)
{
    auto args = std::forward_as_tuple(arguments...);
    std::vector<std::string> args_strings;
    args_strings.reserve(std::tuple_size_v<decltype(args)>);
    std::apply([&args_strings](auto&&... args) {
        (args_strings.emplace_back(convert_to_string(args)), ...);
    }, args);

    return fmt::format("{}", fmt::join(args_strings, " "));
}


class function_call_profiler final
{
public:
    function_call_profiler(const function_call_profiler&) = delete;
    function_call_profiler(function_call_profiler&&) = delete;
    function_call_profiler& operator=(const function_call_profiler&) = delete;
    function_call_profiler& operator=(function_call_profiler&&) = delete;
    ~function_call_profiler() = default;

    template<typename... Args>
    void record_function(const char* name, Args&&... arguments)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_files_holder.getFile(fmt::format("{}.txt", name)) << args_to_string(arguments...) << '\n';
    }

    static function_call_profiler& instance()
    {
        static function_call_profiler instance;
        return instance;
    }
private:
    function_call_profiler() = default;

    std::mutex m_mutex;
    files_holder m_files_holder;
};

} // namespace detail

// #ifdef ENABLE_PROFILING
#define record_function(...) detail::function_call_profiler::instance().record_function(__FUNCTION__ __VA_OPT__(,) __VA_ARGS__)
// #else
// #define record_function(...) ((void)0)
// #endif

}