#ifndef FUNCTION_CALL_EXPRESSION_HPP_INCLUDED
#define FUNCTION_CALL_EXPRESSION_HPP_INCLUDED

#include "expression.hpp"

class FunctionCallExpression : public Expression
{
private:
    std::string function;

public:
    FunctionCallExpression();

    void SetFunction(std::string function);
    std::string_view GetFunction() const;

    void Print(AstPrinter &printer) const override;
};

#endif