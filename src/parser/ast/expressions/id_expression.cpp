#include "id_expression.hpp"

IdExpression::IdExpression() : Expression(AstNodeType::IdExpression) {}

void IdExpression::SetValue(std::string value)
{
    this->value = value;
}

std::string_view IdExpression::GetValue() const
{
    return value;
}

void IdExpression::Print(std::ostream &out, size_t depth) const
{
    const std::string indent(depth * 2, ' ');
    const std::string fieldIndent((depth + 1) * 2, ' ');
    out << indent << "[IdExpression] {\n"
        << fieldIndent << "Value -> " << value << '\n'
        << indent << "}\n";
}
