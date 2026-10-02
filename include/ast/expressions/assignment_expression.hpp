#ifndef ASSIGNMENT_EXPRESSION_HPP_INCLUDED
#define ASSIGNMENT_EXPRESSION_HPP_INCLUDED

#include "expression.hpp"

class AssignmentExpression : public Expression
{
private:
    std::unique_ptr<const Expression> assignee, value;

public:
    AssignmentExpression();

    const std::unique_ptr<const Expression> &GetAssignee() const;
    const std::unique_ptr<const Expression> &GetValue() const;
    void SetAssignee(std::unique_ptr<const Expression> &assignee);
    void SetValue(std::unique_ptr<const Expression> &value);

    void Print(AstPrinter &printer) const;
};

#endif