#ifndef VARIABLE_SYMBOL_HPP_INCLUDED
#define VARIABLE_SYMBOL_HPP_INCLUDED

#include "symbol.hpp"

#include "runtime/runtime_value.hpp"
#include "runtime/null_value.hpp"

class VariableSymbol : public Symbol
{
private:
    std::unique_ptr<RuntimeValue> value;

public:
    VariableSymbol(std::string name, std::string type)
        : Symbol(name, type, SymbolKind::Variable), value(std::make_unique<NullValue>()) {}

    VariableSymbol(std::string name, std::string type, std::unique_ptr<RuntimeValue> value)
        : Symbol(name, type, SymbolKind::Variable), value(std::move(value)) {}

    std::unique_ptr<RuntimeValue> &GetValue();
    void SetValue(std::unique_ptr<RuntimeValue> value);

    void Print(std::ostream &out) const override;
};

#endif
