#ifndef FUNCTION_SYMBOL_HPP_INCLUDED
#define FUNCTION_SYMBOL_HPP_INCLUDED

#include "ast/statements/functions/function_declaration.hpp"
#include "semantic_analysis/symbols/symbol.hpp"
#include <vector>
#include <string>

class FunctionSymbol : public Symbol
{
private:
    const FunctionDeclaration *declaration;
    std::vector<std::string> parameterTypes;

public:
    FunctionSymbol(std::string_view name, std::string_view type);

    void SetFunctionDeclaration(const FunctionDeclaration *declaration);
    const FunctionDeclaration *GetFunctionDeclaration() const;

    void AddParameterType(std::string type);
    const std::vector<std::string> &GetParameterTypes() const;
    size_t GetParameterCount() const;

    void Print(std::ostream &out) const override;
};
#endif