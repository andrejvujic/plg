#include "semantic_analysis/scope.hpp"

Scope::Scope() {}

Scope::Scope(std::shared_ptr<Scope> parent) : parent(parent) {}

std::shared_ptr<Scope> Scope::GetParent() const
{
    return parent;
}

void Scope::DeclareSymbol(std::unique_ptr<const Symbol> &symbol)
{
    const std::string name = std::string(symbol->GetName());
    symbols[name] = std::move(symbol);
}

const std::unique_ptr<const Symbol> &Scope::GetSymbol(std::string_view name) const
{
    auto iterator = symbols.find(std::string(name));
    if (iterator != symbols.end())
        return iterator->second;

    if (parent)
        return parent->GetSymbol(name);

    std::exit(EXIT_FAILURE);
}

bool Scope::SymbolExists(const std::string &name) const
{
    auto iterator = symbols.find(name);
    if (iterator == symbols.end())
        return parent ? parent->SymbolExists(name) : false;

    return true;
}
