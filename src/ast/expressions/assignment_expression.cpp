#include "ast/expressions/assignment_expression.hpp"

AssignmentExpression::AssignmentExpression() : Expression(AstNodeType::AssignmentExpression)
{
}

const std::string &AssignmentExpression::GetVarName() const
{
    return varName;
}

const std::unique_ptr<const Expression> &AssignmentExpression::GetVarValue() const
{
    return varValue;
}

void AssignmentExpression::SetVarName(std::string varName)
{
    this->varName = varName;
}

void AssignmentExpression::SetVarValue(std::unique_ptr<const Expression> &varValue)
{
    this->varValue = std::move(varValue);
}

void AssignmentExpression::Print(AstPrinter &printer) const
{
    printer.BeginNode("AssignmentExpression");
    printer.Field("Var. Name", varName);
    printer.Field("Var. Value", varValue.get());
    printer.EndNode();
}