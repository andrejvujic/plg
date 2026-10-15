#ifndef CALL_EXPRESSION_HPP_INCLUDED
#define CALL_EXPRESSION_HPP_INCLUDED

#include <vector>

#include "ast/expressions/expression.hpp"
#include "ast/statements/functions/function_parameter.hpp"

class CallExpression : public Expression
{
private:
    std::unique_ptr<Expression> callee;
    std::vector<std::unique_ptr<Expression>> arguments;

    void DeclareArgumentBindings(TreeWalkInterpreter &interpreter, const std::vector<std::unique_ptr<FunctionParameter>> &parameters) const;

public:
    CallExpression(SourceLocation location, std::unique_ptr<Expression> callee)
        : Expression(AstNodeType::CallExpression, location), callee(std::move(callee)) {}

    const std::unique_ptr<Expression> &GetCallee() const;

    size_t GetArgumentCount() const;
    const std::vector<std::unique_ptr<Expression>> &GetArguments() const;
    void AddArgument(std::unique_ptr<Expression> argument);

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;
    std::unique_ptr<RuntimeValue> ExecuteNonMemberFunction(TreeWalkInterpreter &interpreter) const;

    void Print(AstPrinter &printer) const override;
};

#endif
