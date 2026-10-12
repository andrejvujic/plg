#include "runtime/null_value.hpp"

std::unique_ptr<RuntimeValue> NullValue::Clone() const
{
    return std::make_unique<NullValue>();
}

std::ostream &NullValue::Print(std::ostream &out) const
{
    return out << "null";
}
