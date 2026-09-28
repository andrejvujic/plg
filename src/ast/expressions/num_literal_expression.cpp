#include "ast/expressions/num_literal_expression.hpp"

NumLiteralExpression::NumLiteralExpression() : Expression(AstNodeType::NumLiteralExpression) {}

void NumLiteralExpression::SetRawValue(std::string rawValue)
{
    this->rawValue = rawValue;
}

std::string_view NumLiteralExpression::GetRawValue() const
{
    return rawValue;
}

int NumLiteralExpression::GetValue() const
{
    return atoi(rawValue.c_str());
}

void NumLiteralExpression::Print(AstPrinter &printer) const
{
    printer.BeginNode("NumLiteralExpression");
    printer.Field("Raw Value", rawValue);
    printer.EndNode();
}
