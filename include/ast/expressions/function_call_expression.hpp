#ifndef FUNCTION_CALL_EXPRESSION_HPP_INCLUDED
#define FUNCTION_CALL_EXPRESSION_HPP_INCLUDED

#include <vector>

#include "expression.hpp"

class FunctionCallExpression : public Expression
{
private:
    std::string callee;
    std::vector<std::unique_ptr<Expression>> arguments;

public:
    FunctionCallExpression(SourceLocation location, std::string callee)
        : Expression(AstNodeType::FunctionCallExpression, location), callee(std::move(callee)) {}

    const std::string &GetCallee() const;

    size_t GetArgumentCount() const;
    const std::vector<std::unique_ptr<Expression>> &GetArguments() const;
    void AddArgument(std::unique_ptr<Expression> argument);

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;
};

#endif
