#include "ast/statements/functions/function_parameter.hpp"

const std::string &FunctionParameter::GetVarName() const
{
    return varName;
}

const std::string &FunctionParameter::GetVarType() const
{
    return varType;
}

void FunctionParameter::Print(AstPrinter &printer) const
{
    printer.BeginNode("FunctionParameter");
    printer.Field("Var. Type", varType);
    printer.Field("Var. Name", varName);
    printer.EndNode();
}

std::unique_ptr<RuntimeValue> FunctionParameter::Execute(TreeWalkInterpreter &) const
{
    return nullptr;
}
