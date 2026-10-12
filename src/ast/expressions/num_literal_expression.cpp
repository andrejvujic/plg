#include "ast/expressions/num_literal_expression.hpp"
#include "runtime/int_value.hpp"
#include "runtime/unsigned_int_value.hpp"
#include "shared/types.hpp"

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

std::unique_ptr<RuntimeValue> NumLiteralExpression::Execute(TreeWalkInterpreter &interpreter) const
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