#ifndef IDENTIFIER_EXPRESSION_HPP_INCLUD
#define IDENTIFIER_EXPRESSION_HPP_INCLUD

#include "ast/expressions/expression.hpp"

class IdExpression : public Expression
{
private:
    std::string value;

public:
    IdExpression();

    std::string_view GetValue() const;
    void SetValue(std::string value);

    void Print(AstPrinter &printer) const override;
};

#endif
