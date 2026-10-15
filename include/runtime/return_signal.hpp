#ifndef RETURN_SIGNAL_HPP_INCLUDED
#define RETURN_SIGNAL_HPP_INCLUDED

#include <memory>
#include "runtime/values/runtime_value.hpp"

class ReturnSignal
{
private:
    std::unique_ptr<RuntimeValue> value;

public:
    ReturnSignal() = default;
    ReturnSignal(std::unique_ptr<RuntimeValue> value) : value(std::move(value)) {}
    const std::unique_ptr<RuntimeValue> &GetValue() const;
};

#endif
