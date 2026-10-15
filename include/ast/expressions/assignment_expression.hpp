#ifndef ASSIGNMENT_EXPRESSION_HPP_INCLUDED
#define ASSIGNMENT_EXPRESSION_HPP_INCLUDED

#include "expression.hpp"

class AssignmentExpression : public Expression
{
private:
    std::unique_ptr<Expression> assignee, value;

public:
    AssignmentExpression();

    const std::unique_ptr<Expression> &GetAssignee() const;
    const std::unique_ptr<Expression> &GetValue() const;
    void SetAssignee(std::unique_ptr<Expression> &assignee);
    void SetValue(std::unique_ptr<Expression> &value);

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;
};

#endif
