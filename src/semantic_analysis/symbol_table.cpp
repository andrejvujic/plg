#include "semantic_analysis/symbol_table.hpp"

std::shared_ptr<SymbolTable> SymbolTable::GetParent() const
{
    return parent;
}

bool SymbolTable::HasParent() const
{
    return parent != nullptr;
}

void SymbolTable::DeclareSymbol(std::unique_ptr<Symbol> symbol)
{
    const auto name = symbol->GetName();
    symbols[name] = std::move(symbol);
}

std::unique_ptr<Symbol> &SymbolTable::GetSymbol(const std::string &name, SymbolKind kind)
{
    auto iterator = symbols.find(name);
    if (iterator != symbols.end() && iterator->second->IsOfKind(kind))
        return iterator->second;

    if (parent)
        return parent->GetSymbol(name, kind);

    std::cerr << "[SymbolTable] ERROR: Symbol with given name and kind doesn't exist" << std::endl;
    std::exit(EXIT_FAILURE);
}

std::unique_ptr<Symbol> &SymbolTable::GetSymbol(const std::string &name, SymbolKind kind, size_t distance)
{
    auto table = GetAncestor(distance);
    auto iterator = table->symbols.find(name);
    if (iterator != table->symbols.end() && iterator->second->IsOfKind(kind))
        return iterator->second;

    std::cerr << "[SymbolTable] ERROR: Symbol with given name and kind doesn't exist at given distance" << std::endl;
    std::exit(EXIT_FAILURE);
}

bool SymbolTable::SymbolExists(const std::string &name, SymbolKind kind) const
{
    auto iterator = symbols.find(name);
    if (iterator != symbols.end() && iterator->second->IsOfKind(kind))
        return true;

    return parent ? parent->SymbolExists(name, kind) : false;
}

bool SymbolTable::SymbolExists(const std::string &name, SymbolKind kind, size_t distance) const
{
    const auto table = GetAncestor(distance);
    auto iterator = table->symbols.find(name);
    return iterator != table->symbols.end() && iterator->second->IsOfKind(kind);
}

SymbolTable *SymbolTable::GetAncestor(size_t distance)
{
    auto table = this;
    while (distance-- > 0)
    {
        if (!table->parent)
            return table;
        table = table->parent.get();
    }
    return table;
}

const SymbolTable *SymbolTable::GetAncestor(size_t distance) const
{
    auto table = this;
    while (distance-- > 0)
    {
        if (!table->parent)
            return table;
        table = table->parent.get();
    }
    return table;
}
