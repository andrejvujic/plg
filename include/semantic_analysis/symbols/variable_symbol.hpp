#ifndef VARIABLE_SYMBOL_HPP_INCLUDED
#define VARIABLE_SYMBOL_HPP_INCLUDED

#include "semantic_analysis/symbols/symbol.hpp"

class VariableSymbol : public Symbol
{
public:
    VariableSymbol(std::string name, std::string type)
        : Symbol(name, type, SymbolKind::Variable) {}

    void Print(std::ostream &out) const override;
};

#endif
