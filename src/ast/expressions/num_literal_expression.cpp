#include "ast/expressions/num_literal_expression.hpp"
#include "runtime/values/int_value.hpp"
#include "runtime/values/unsigned_int_value.hpp"
#include "shared/types.hpp"

const std::string &NumLiteralExpression::GetRawValue() const
{
    return rawValue;
}

const std::string &NumLiteralExpression::GetLiteralType() const
{
    return literalType;
}

void NumLiteralExpression::Print(AstPrinter &printer) const
{
    printer.BeginNode("NumLiteralExpression");
    printer.Field("Raw Value", rawValue);
    printer.Field("Literal Type", literalType);
    printer.EndNode();
}

std::unique_ptr<RuntimeValue> NumLiteralExpression::Execute(TreeWalkInterpreter &) const
{
    if (literalType == Types::Int)
    {
        auto value = std::stoi(rawValue);
        return std::make_unique<IntValue>(value);
    }

    if (literalType == Types::UsignedInt)
    {
        // This might throw?
        auto value = static_cast<unsigned int>(std::stoul(rawValue));
        return std::make_unique<UnsignedIntValue>(value);
    }

    std::cerr << "[NumLiteralExpression] ERROR: Couldn't create RuntimeValue for literal type " << literalType << std::endl;
    std::exit(EXIT_FAILURE);
}
