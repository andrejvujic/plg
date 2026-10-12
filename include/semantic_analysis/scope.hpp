#ifndef SCOPE_HPP_INCLUDED
#define SCOPE_HPP_INCLUDED

#include <unordered_map>
#include <string>
#include <string_view>
#include <iostream>

#include "semantic_analysis/symbols/symbol.hpp"

class Scope
{
private:
    std::shared_ptr<Scope> parent;
    std::unordered_map<std::string, std::unique_ptr<Symbol>> symbols;

public:
    Scope();
    Scope(std::shared_ptr<Scope> parent);

    std::shared_ptr<Scope> GetParent() const;
    bool HasParent() const;

    void DeclareSymbol(std::unique_ptr<Symbol> &symbol);
    std::unique_ptr<Symbol> &GetSymbol(const std::string &name, SymbolKind kind);
    bool SymbolExists(const std::string &name, SymbolKind kind) const;

    void Print(std::ostream &out) const;
};

#endif
