#include "ast/statements/expression_statement.hpp"

ExpressionStatement::ExpressionStatement() : Statement(AstNodeType::ExpressionStatement)
{
}

const std::unique_ptr<const Expression> &ExpressionStatement::GetExpression() const
{
    return expression;
};

void ExpressionStatement::SetExpression(std::unique_ptr<const Expression> &expression)
{
    this->expression = std::move(expression);
}

void ExpressionStatement::Print(AstPrinter &printer) const
{
    printer.BeginNode("ExpressionStatement");
    printer.Field("Expression", expression.get());
    printer.EndNode();
}

std::unique_ptr<RuntimeValue> ExpressionStatement::Execute(TreeWalkInterpreter &interpreter) const
{
    return nullptr;
}