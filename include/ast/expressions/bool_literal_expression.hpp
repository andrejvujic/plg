#ifndef BOOL_LITERAL_EXPRESSION_HPP_INCLUD
#define BOOL_LITERAL_EXPRESSION_HPP_INCLUD

#include "ast/expressions/expression.hpp"

class BoolLiteralExpression : public Expression
{
private:
    std::string rawValue;

public:
    BoolLiteralExpression(SourceLocation location, std::string rawValue)
        : Expression(AstNodeType::BoolLiteralExpression, location), rawValue(std::move(rawValue)) {}

    const std::string &GetRawValue() const;

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;

    constexpr static const std::string_view True = "true";
    constexpr static const std::string_view False = "false";
};

#endif
