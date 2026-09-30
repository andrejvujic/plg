#ifndef SCOPE_HPP_INCLUDED
#define SCOPE_HPP_INCLUDED

#include <unordered_map>
#include <string>
#include <string_view>

#include "semantic_analysis/symbols/symbol.hpp"

class Scope
{
private:
    std::shared_ptr<Scope> parent;
    std::unordered_map<std::string, std::unique_ptr<const Symbol>> symbols;

public:
    Scope();
    Scope(std::shared_ptr<Scope> parent);

    std::shared_ptr<Scope> GetParent() const;

    void DeclareSymbol(std::unique_ptr<const Symbol> &symbol);
    const std::unique_ptr<const Symbol> &GetSymbol(std::string_view name) const;
    bool SymbolExists(const std::string &name) const;
};

#endif
