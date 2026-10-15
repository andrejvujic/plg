#include "runtime/values/unsigned_int_value.hpp"

unsigned int UnsignedIntValue::GetValue() const
{
    return value;
}

std::unique_ptr<RuntimeValue> UnsignedIntValue::Clone() const
{
    return std::make_unique<UnsignedIntValue>(value);
}

std::ostream &UnsignedIntValue::Print(std::ostream &out) const
{
    return out << value;
}
