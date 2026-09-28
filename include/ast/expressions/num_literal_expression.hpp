#ifndef NUM_LITERAL_EXPRESSION_HPP_INCLUD
#define NUM_LITERAL_EXPRESSION_HPP_INCLUD

#include "ast/expressions/expression.hpp"

class NumLiteralExpression : public Expression
{
private:
    std::string rawValue;

public:
    NumLiteralExpression();

    int GetValue() const;
    std::string_view GetRawValue() const;
    void SetRawValue(std::string rawValue);

    void Print(AstPrinter &printer) const override;
};

#endif
