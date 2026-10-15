#include "semantic_analysis/symbols/function_symbol.hpp"

FunctionSymbol::FunctionSymbol(std::string_view name,
                               std::string_view type,
                               const FunctionDeclaration &declaration)
    : Symbol(name, type, SymbolKind::Function)
{
    for (const auto &parameter : declaration.GetParameters())
        parameterTypes.push_back(parameter->GetVarType());
}

const std::vector<std::string> &FunctionSymbol::GetParameterTypes() const
{
    return parameterTypes;
}

size_t FunctionSymbol::GetParameterCount() const
{
    return parameterTypes.size();
}

void FunctionSymbol::Print(std::ostream &out) const
{
    out << this->GetType() << " " << this->GetName() << "(";
    for (size_t index = 0; index < parameterTypes.size(); ++index)
    {
        if (index < parameterTypes.size() - 1)
            out << parameterTypes[index] << ", ";
        else
            out << parameterTypes[index];
    }
    out << ")" << std::endl;
}
