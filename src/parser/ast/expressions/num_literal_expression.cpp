#include "num_literal_expression.hpp"

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

void NumLiteralExpression::Print(std::ostream &out, size_t depth) const
{
    const std::string indent(depth * 2, ' ');
    const std::string fieldIndent((depth + 1) * 2, ' ');
    out << indent << "[NumLiteralExpression] {\n"
        << fieldIndent << "Raw Value -> " << rawValue << '\n'
        << indent << "}\n";
}
