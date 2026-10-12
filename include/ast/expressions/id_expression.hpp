#ifndef IDENTIFIER_EXPRESSION_HPP_INCLUD
#define IDENTIFIER_EXPRESSION_HPP_INCLUD

#include "ast/expressions/expression.hpp"

class IdExpression : public Expression
{
private:
    std::string value;

public:
    IdExpression();

    const std::string &GetValue() const;
    void SetValue(std::string value);

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;
};

#endif
