#include "runtime/bindings/variable_binding.hpp"

std::unique_ptr<RuntimeValue> &VariableBinding::GetValue()
{
    return value;
}

void VariableBinding::SetValue(std::unique_ptr<RuntimeValue> value)
{
    this->value = std::move(value);
}

void VariableBinding::Print(std::ostream &out) const
{
    out << GetType() << " " << GetName() << " = ";
    value->Print(out) << std::endl;
}
