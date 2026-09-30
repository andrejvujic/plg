#include "semantic_analysis/scope.hpp"
#include <iostream>

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

// TODO: Return multiple symbols for SymbolKind::Function to support function overloading?
const std::unique_ptr<const Symbol> &Scope::GetSymbol(const std::string &name, SymbolKind kind) const
{
    for (auto iterator = symbols.begin(); iterator != symbols.end(); ++iterator)
    {
        const auto &symbol = iterator->second;
        if (symbol->GetName() == name && symbol->IsOfKind(kind))
            return symbol;
    }

    if (parent)
        return parent->GetSymbol(name, kind);

    std::cerr << "[Scope] ERROR: Symbol with given name and kind doesn't exist, always call SymbolExists() before calling GetSymbol()" << std::endl;
    std::exit(EXIT_FAILURE);
}

bool Scope::SymbolExists(const std::string &name, SymbolKind kind) const
{
    for (auto iterator = symbols.begin(); iterator != symbols.end(); ++iterator)
    {
        const auto &symbol = iterator->second;
        if (symbol->GetName() == name && symbol->IsOfKind(kind))
            return true;
    }

    return parent ? parent->SymbolExists(name, kind) : false;
}
