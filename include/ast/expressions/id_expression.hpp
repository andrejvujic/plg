#ifndef IDENTIFIER_EXPRESSION_HPP_INCLUD
#define IDENTIFIER_EXPRESSION_HPP_INCLUD

#include "ast/expressions/expression.hpp"

class IdExpression : public Expression
{
private:
    std::string value;

public:
    IdExpression(SourceLocation location, std::string value)
        : Expression(AstNodeType::IdExpression, location), value(std::move(value)) {}

    const std::string &GetValue() const;

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;
};

#endif
