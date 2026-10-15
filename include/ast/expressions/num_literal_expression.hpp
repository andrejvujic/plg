#ifndef NUM_LITERAL_EXPRESSION_HPP_INCLUD
#define NUM_LITERAL_EXPRESSION_HPP_INCLUD

#include "ast/expressions/expression.hpp"

class NumLiteralExpression : public Expression
{
private:
    std::string rawValue, literalType;

public:
    NumLiteralExpression(SourceLocation location, std::string rawValue, std::string literalType)
        : Expression(AstNodeType::NumLiteralExpression, location),
          rawValue(std::move(rawValue)),
          literalType(std::move(literalType)) {}

    const std::string &GetLiteralType() const;

    const std::string &GetRawValue() const;

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;
};

#endif
