#include "ast/expressions/bool_literal_expression.hpp"

#include "shared/types.hpp"
#include "runtime/bool_value.hpp"

const std::string &BoolLiteralExpression::GetRawValue() const
{
    return rawValue;
}

void BoolLiteralExpression::Print(AstPrinter &printer) const
{
    printer.BeginNode("BoolLiteralExpression");
    printer.Field("Raw Value", rawValue);
    printer.EndNode();
}

std::unique_ptr<RuntimeValue> BoolLiteralExpression::Execute(TreeWalkInterpreter &) const
{
    return std::make_unique<BoolValue>(rawValue == BoolLiteralExpression::True);
}
