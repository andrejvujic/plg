#include "ast/expressions/num_literal_expression.hpp"

NumLiteralExpression::NumLiteralExpression() : Expression(AstNodeType::NumLiteralExpression) {}

void NumLiteralExpression::SetRawValue(std::string rawValue)
{
    this->rawValue = rawValue;
}

const std::string &NumLiteralExpression::GetRawValue() const
{
    return rawValue;
}

const std::string &NumLiteralExpression::GetLiteralType() const
{
    return literalType;
}

void NumLiteralExpression::SetLiteralType(std::string literalType)
{
    this->literalType = literalType;
}

void NumLiteralExpression::Print(AstPrinter &printer) const
{
    printer.BeginNode("NumLiteralExpression");
    printer.Field("Raw Value", rawValue);
    printer.Field("Literal Type", literalType);
    printer.EndNode();
}
