#ifndef NUM_LITERAL_EXPRESSION_HPP_INCLUD
#define NUM_LITERAL_EXPRESSION_HPP_INCLUD

#include "ast/expressions/expression.hpp"

class NumLiteralExpression : public Expression
{
private:
    std::string rawValue, literalType;

public:
    NumLiteralExpression();

    const std::string &GetLiteralType() const;
    void SetLiteralType(std::string literalType);

    const std::string &GetRawValue() const;
    void SetRawValue(std::string rawValue);

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;
};

#endif
