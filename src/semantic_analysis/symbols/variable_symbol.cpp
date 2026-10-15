#include "semantic_analysis/symbols/variable_symbol.hpp"

void VariableSymbol::Print(std::ostream &out) const
{
    out << GetType() << " " << GetName() << std::endl;
}
