#ifndef SCOPE_HPP_INCLUDED
#define SCOPE_HPP_INCLUDED

#include <unordered_map>
#include <string>
#include <string_view>
#include <iostream>
#include <utility>

#include "semantic_analysis/symbols/symbol.hpp"

class Scope
{
private:
    std::shared_ptr<Scope> parent;
    std::unordered_map<std::string, std::unique_ptr<Symbol>> symbols;

public:
    Scope() = default;
    Scope(std::shared_ptr<Scope> parent) : parent(std::move(parent)) {}

    std::shared_ptr<Scope> GetParent() const;
    bool HasParent() const;

    void DeclareSymbol(std::unique_ptr<Symbol> symbol);
    std::unique_ptr<Symbol> &GetSymbol(const std::string &name, SymbolKind kind);
    std::unique_ptr<Symbol> &GetSymbol(const std::string &name, SymbolKind kind, size_t distance);
    bool SymbolExists(const std::string &name, SymbolKind kind) const;
    bool SymbolExists(const std::string &name, SymbolKind kind, size_t distance);

    constexpr static size_t Local = 0, Parent = 1;

    void Print(std::ostream &out) const;
};

#endif
