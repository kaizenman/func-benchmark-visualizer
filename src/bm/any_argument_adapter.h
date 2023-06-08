#pragma once

#include <functional>
#include <vector>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <variant>

using arg_type = std::variant<int, double, std::string>;

template <typename Ret, typename... Args>
Ret callfunc (std::function<Ret(Args...)> func, std::vector<arg_type> anyargs);

template <typename Ret>
Ret callfunc (std::function<Ret()> func, std::vector<arg_type> anyargs)
{
    if (anyargs.size() > 0)
        throw std::runtime_error("oops, argument list too long");
    return func();
}

template <typename Ret, typename Arg0, typename... Args>
Ret callfunc (std::function<Ret(Arg0, Args...)> func, std::vector<arg_type> anyargs)
{
    if (anyargs.size() == 0)
        throw std::runtime_error("oops, argument list too short");
    Arg0 arg0 = std::get<Arg0>(anyargs[0]);
    anyargs.erase(anyargs.begin());
    std::function<Ret(Args... args)> lambda =
        ([=](Args... args) -> Ret {
         return func(arg0, args...);
    });
    return callfunc (lambda, anyargs);
}

template <typename Ret, typename... Args>
std::function<arg_type(std::vector<arg_type>)> adaptfunc (Ret (*func)(Args...)) {
    std::function<Ret(Args...)> stdfunc = func;
    std::function<arg_type(std::vector<arg_type>)> result =
        ([=](std::vector<arg_type> anyargs) -> arg_type {
         return arg_type(callfunc(stdfunc, anyargs));
         });
    return result;
}
