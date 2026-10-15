#ifndef FUNCTION_PARAMETER_HPP_INCLUDED
#define FUNCTION_PARAMETER_HPP_INCLUDED

#include "ast/ast.hpp"

class FunctionParameter : public AstNode
{
private:
    std::string varType;
    std::string varName;

public:
    FunctionParameter(SourceLocation location, std::string varType, std::string varName)
        : AstNode(AstNodeType::FunctionParameter, location),
          varType(std::move(varType)),
          varName(std::move(varName)) {}

    const std::string &GetVarName() const;
    const std::string &GetVarType() const;

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;
};

#endif
