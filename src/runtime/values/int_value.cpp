#include "runtime/values/int_value.hpp"

int IntValue::GetValue() const
{
    return value;
}

std::unique_ptr<RuntimeValue> IntValue::Clone() const
{
    return std::make_unique<IntValue>(value);
}

std::ostream &IntValue::Print(std::ostream &out) const
{
    return out << value;
}
