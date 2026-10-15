#ifndef SYMBOL_TABLE_HPP_INCLUDED
#define SYMBOL_TABLE_HPP_INCLUDED

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "semantic_analysis/symbols/symbol.hpp"

class SymbolTable
{
private:
    std::shared_ptr<SymbolTable> parent;
    std::unordered_map<std::string, std::unique_ptr<Symbol>> symbols;

    SymbolTable *GetAncestor(size_t distance);
    const SymbolTable *GetAncestor(size_t distance) const;

public:
    SymbolTable() = default;
    SymbolTable(std::shared_ptr<SymbolTable> parent) : parent(std::move(parent)) {}

    std::shared_ptr<SymbolTable> GetParent() const;
    bool HasParent() const;

    void DeclareSymbol(std::unique_ptr<Symbol> symbol);
    std::unique_ptr<Symbol> &GetSymbol(const std::string &name, SymbolKind kind);
    std::unique_ptr<Symbol> &GetSymbol(const std::string &name, SymbolKind kind, size_t distance);
    bool SymbolExists(const std::string &name, SymbolKind kind) const;
    bool SymbolExists(const std::string &name, SymbolKind kind, size_t distance) const;

    constexpr static size_t Local = 0, Parent = 1;
};

#endif
