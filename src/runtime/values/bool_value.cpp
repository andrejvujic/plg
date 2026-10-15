#include "runtime/values/bool_value.hpp"

bool BoolValue::GetValue() const
{
    return value;
}

std::unique_ptr<RuntimeValue> BoolValue::Clone() const
{
    return std::make_unique<BoolValue>(value);
}

std::ostream &BoolValue::Print(std::ostream &out) const
{
    return out << (value ? "true" : "false");
}
