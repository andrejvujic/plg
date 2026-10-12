#include "ast/expressions/assignment_expression.hpp"

AssignmentExpression::AssignmentExpression() : Expression(AstNodeType::AssignmentExpression)
{
}

const std::unique_ptr<const Expression> &AssignmentExpression::GetAssignee() const
{
    return assignee;
}

const std::unique_ptr<const Expression> &AssignmentExpression::GetValue() const
{
    return value;
}

void AssignmentExpression::SetAssignee(std::unique_ptr<const Expression> &assignee)
{
    this->assignee = std::move(assignee);
}

void AssignmentExpression::SetValue(std::unique_ptr<const Expression> &value)
{
    this->value = std::move(value);
}

void AssignmentExpression::Print(AstPrinter &printer) const
{
    printer.BeginNode("AssignmentExpression");
    printer.Field("Assignee", assignee.get());
    printer.Field("Value", value.get());
    printer.EndNode();
}

std::unique_ptr<RuntimeValue> AssignmentExpression::Execute(TreeWalkInterpreter &interpreter) const
{
    return nullptr;
}