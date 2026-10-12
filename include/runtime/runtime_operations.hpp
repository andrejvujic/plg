#ifndef RUNTIME_OPERATIONS_HPP_INCLUDED
#define RUNTIME_OPERATIONS_HPP_INCLUDED

#include "runtime/runtime_value.hpp"

class RuntimeOperations
{
public:
    static std::unique_ptr<RuntimeValue> Add(std::unique_ptr<RuntimeValue> &a, std::unique_ptr<RuntimeValue> &b);
};

#endif