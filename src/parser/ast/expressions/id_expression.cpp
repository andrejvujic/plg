#include "parser/ast/expressions/id_expression.hpp"

IdExpression::IdExpression() : Expression(AstNodeType::IdExpression) {}

void IdExpression::SetValue(std::string value)
{
    this->value = value;
}

std::string_view IdExpression::GetValue() const
{
    return value;
}

void IdExpression::Print(AstPrinter &printer) const
{
    printer.BeginNode("IdExpression");
    printer.Field("Value", value);
    printer.EndNode();
}
