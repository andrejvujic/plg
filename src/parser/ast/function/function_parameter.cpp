#include "parser/ast/function/function_parameter.hpp"

FunctionParameter::FunctionParameter() : AstNode(AstNodeType::FunctionParameter)
{
}

std::string_view FunctionParameter::GetVarName() const
{
    return varName;
}

std::string_view FunctionParameter::GetVarType() const
{
    return varType;
}

void FunctionParameter::SetVarName(std::string varName)
{
    this->varName = varName;
}

void FunctionParameter::SetVarType(std::string varType)
{
    this->varType = varType;
}

void FunctionParameter::Print(AstPrinter &printer) const
{
    printer.BeginNode("FunctionParameter");
    printer.Field("Var. Type", varType);
    printer.Field("Var. Name", varName);
    printer.EndNode();
}