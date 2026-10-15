#ifndef FUNCTION_CALL_EXPRESSION_HPP_INCLUDED
#define FUNCTION_CALL_EXPRESSION_HPP_INCLUDED

#include <vector>

#include "expression.hpp"

class FunctionCallExpression : public Expression
{
private:
    std::string callee;
    std::vector<std::unique_ptr<const Expression>> arguments;

public:
    FunctionCallExpression();

    void SetCallee(std::string callee);
    const std::string &GetCallee() const;

    size_t GetArgumentCount() const;
    const std::vector<std::unique_ptr<const Expression>> &GetArguments() const;

    void AddArgument(std::unique_ptr<const Expression> &argument);

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;
};

#endif