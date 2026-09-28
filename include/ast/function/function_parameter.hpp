#ifndef FUNCTION_PARAMETER_HPP_INCLUDED
#define FUNCTION_PARAMETER_HPP_INCLUDED

#include "ast/ast.hpp"

class FunctionParameter : public AstNode
{
private:
    std::string varType;
    std::string varName;

public:
    FunctionParameter();

    std::string_view GetVarName() const;
    std::string_view GetVarType() const;

    void SetVarName(std::string varName);
    void SetVarType(std::string varType);

    void Print(AstPrinter &printer) const override;
};

#endif