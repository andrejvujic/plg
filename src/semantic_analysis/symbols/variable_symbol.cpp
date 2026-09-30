#include "semantic_analysis/symbols/variable_symbol.hpp"

VariableSymbol::VariableSymbol(std::string name, std::string type) : Symbol(name, type, SymbolKind::Variable)
{
}