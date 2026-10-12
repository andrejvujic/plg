#ifndef EXPRESSION_STATEMENT_HPP_INCLUDED
#define EXPRESSION_STATEMENT_HPP_INCLUDED

#include "statement.hpp"
#include "ast/expressions/expression.hpp"
#include "ast/ast_printer.hpp"

class ExpressionStatement : public Statement
{
private:
    std::unique_ptr<const Expression> expression;

public:
    ExpressionStatement();

    const std::unique_ptr<const Expression> &GetExpression() const;
    void SetExpression(std::unique_ptr<const Expression> &expression);

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;
};

#endif