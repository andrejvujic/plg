#include "runtime/return_signal.hpp"

const std::unique_ptr<RuntimeValue> &ReturnSignal::GetValue() const
{
    return value;
}