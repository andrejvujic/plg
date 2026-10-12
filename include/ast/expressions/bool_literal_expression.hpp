#ifndef BOOL_LITERAL_EXPRESSION_HPP_INCLUD
#define BOOL_LITERAL_EXPRESSION_HPP_INCLUD

#include "ast/expressions/expression.hpp"

class BoolLiteralExpression : public Expression
{
private:
    std::string rawValue;

public:
    BoolLiteralExpression();

    const std::string &GetRawValue() const;
    void SetRawValue(std::string rawValue);

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;

    constexpr static const std::string_view BOOL_TRUE = "true";
    constexpr static const std::string_view BOOL_FALSE = "false";
};

#endif
