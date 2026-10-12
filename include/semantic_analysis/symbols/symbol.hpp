#ifndef SYMBOL_HPP_INCLUDED
#define SYMBOL_HPP_INCLUDED

#include <string>
#include <string_view>
#include <iostream>

enum class SymbolKind
{
    Function,
    Variable,
};

class Symbol
{
private:
    std::string name, type;
    SymbolKind kind;

public:
    Symbol(std::string_view name, std::string_view type, SymbolKind kind);

    const std::string &GetName() const;
    const std::string &GetType() const;
    SymbolKind GetKind() const;

    bool IsOfKind(SymbolKind kind) const;
    bool IsOfType(std::string_view type) const;

    virtual void Print(std::ostream &out) const = 0;

    virtual ~Symbol() = default;
};

#endif