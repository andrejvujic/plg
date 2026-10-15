#include "symbols/variable_symbol.hpp"

std::unique_ptr<RuntimeValue> &VariableSymbol::GetValue()
{
    return value;
}

void VariableSymbol::SetValue(std::unique_ptr<RuntimeValue> value)
{
    this->value = std::move(value);
}

void VariableSymbol::Print(std::ostream &out) const
{
    out << this->GetType() << " " << this->GetName() << " = ";
    value->Print(out) << std::endl;
}
