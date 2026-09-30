#ifndef ASSIGNMENT_EXPRESSION_HPP_INCLUDED
#define ASSIGNMENT_EXPRESSION_HPP_INCLUDED

#include "expression.hpp"

class AssignmentExpression : public Expression
{
private:
    std::string varName;
    std::unique_ptr<const Expression> varValue;

public:
    AssignmentExpression();

    const std::string &GetVarName() const;
    const std::unique_ptr<const Expression> &GetVarValue() const;
    void SetVarName(std::string varName);
    void SetVarValue(std::unique_ptr<const Expression> &varValue);

    void Print(AstPrinter &printer) const;
};

#endif