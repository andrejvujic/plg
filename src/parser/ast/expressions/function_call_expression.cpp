#include "parser/ast/expressions/function_call_expression.hpp"

FunctionCallExpression::FunctionCallExpression() : Expression(AstNodeType::FunctionCallExpression)
{
}

std::string_view FunctionCallExpression::GetFunction() const
{
    return function;
}

void FunctionCallExpression::SetFunction(std::string function)
{
    this->function = function;
}

void FunctionCallExpression::Print(AstPrinter &printer) const
{
    printer.BeginNode("FunctionCallExpression");
    printer.Field("Function", function);
    printer.EndNode();
}