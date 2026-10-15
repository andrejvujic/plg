#ifndef ASSIGNMENT_EXPRESSION_HPP_INCLUDED
#define ASSIGNMENT_EXPRESSION_HPP_INCLUDED

#include "expression.hpp"

class AssignmentExpression : public Expression
{
private:
    std::unique_ptr<Expression> assignee, value;

public:
    AssignmentExpression(SourceLocation location,
                         std::unique_ptr<Expression> assignee,
                         std::unique_ptr<Expression> value)
        : Expression(AstNodeType::AssignmentExpression, location),
          assignee(std::move(assignee)),
          value(std::move(value)) {}

    const std::unique_ptr<Expression> &GetAssignee() const;
    const std::unique_ptr<Expression> &GetValue() const;

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;
};

#endif
