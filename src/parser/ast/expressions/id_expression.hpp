#ifndef IDENTIFIER_EXPRESSION_HPP_INCLUD
#define IDENTIFIER_EXPRESSION_HPP_INCLUD

#include "expression.hpp"

class IdExpression : public Expression
{
private:
    std::string value;

public:
    IdExpression();

    std::string_view GetValue() const;
    void SetValue(std::string value);

    void Print(std::ostream &out, size_t depth = 0) const override;
};

#endif
