#ifndef FUNCTION_SYMBOL_HPP_INCLUDED
#define FUNCTION_SYMBOL_HPP_INCLUDED

#include "semantic_analysis/symbols/symbol.hpp"
#include <vector>
#include <string>

class FunctionSymbol : public Symbol
{
private:
    std::vector<std::string> parameters;

public:
    FunctionSymbol(std::string_view name, std::string_view type);

    void AddParameter(std::string type);
    const std::vector<std::string> &GetParameters() const;
    size_t GetParameterCount() const;
};
#endif