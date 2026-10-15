#ifndef RUNTIME_OPERATIONS_HPP_INCLUDED
#define RUNTIME_OPERATIONS_HPP_INCLUDED

#include "runtime/runtime_value.hpp"

class RuntimeOperations
{
public:
    static std::unique_ptr<RuntimeValue> Add(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b);
    static std::unique_ptr<RuntimeValue> Sub(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b);
    static std::unique_ptr<RuntimeValue> Mul(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b);
    static std::unique_ptr<RuntimeValue> Div(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b);

    static std::unique_ptr<RuntimeValue> GreaterThan(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b);
    static std::unique_ptr<RuntimeValue> GreaterThenOrEq(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b);
    static std::unique_ptr<RuntimeValue> LessThan(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b);
    static std::unique_ptr<RuntimeValue> LessThenOrEq(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b);
    static std::unique_ptr<RuntimeValue> Eq(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b);
    static std::unique_ptr<RuntimeValue> NotEq(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b);

    static std::unique_ptr<RuntimeValue> And(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b);
    static std::unique_ptr<RuntimeValue> Or(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b);
};

#endif