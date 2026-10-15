#ifndef EXPRESSION_STATEMENT_HPP_INCLUDED
#define EXPRESSION_STATEMENT_HPP_INCLUDED

#include "statement.hpp"
#include "ast/expressions/expression.hpp"
#include "ast/ast_printer.hpp"

class ExpressionStatement : public Statement
{
private:
    std::unique_ptr<Expression> expression;

public:
    ExpressionStatement(SourceLocation location, std::unique_ptr<Expression> expression)
        : Statement(AstNodeType::ExpressionStatement, location), expression(std::move(expression)) {}

    const std::unique_ptr<Expression> &GetExpression() const;

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;
};

#endif
