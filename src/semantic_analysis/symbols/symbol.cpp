#include "semantic_analysis/symbols/symbol.hpp"

Symbol::Symbol(std::string_view name, std::string_view type, SymbolKind kind) : name(std::string(name)), type(std::string(type)), kind(kind) {}

const std::string &Symbol::GetName() const
{
    return name;
}

const std::string &Symbol::GetType() const
{
    return type;
}

SymbolKind Symbol::GetKind() const
{
    return kind;
}

bool Symbol::IsOfKind(SymbolKind kind) const
{
    return this->kind == kind;
}

bool Symbol::IsOfType(std::string_view type) const
{
    return this->type == type;
}