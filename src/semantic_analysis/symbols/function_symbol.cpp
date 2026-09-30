#include "semantic_analysis/symbols/function_symbol.hpp"

FunctionSymbol::FunctionSymbol(std::string_view name, std::string_view type) : Symbol(name, type, SymbolKind::Function)
{
}

void FunctionSymbol::AddParameter(std::string type)
{
    parameters.push_back(type);
}

const std::vector<std::string> &FunctionSymbol::GetParameters() const
{
    return parameters;
}

size_t FunctionSymbol::GetParameterCount() const
{
    return parameters.size();
}