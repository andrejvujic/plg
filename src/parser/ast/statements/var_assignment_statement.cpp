#include "parser/ast/statements/var_assignment_statement.hpp"

VarAssignmentStatement::VarAssignmentStatement() : Statement(AstNodeType::VarAssignmentStatement)
{
}

std::string_view VarAssignmentStatement::GetVarName() const
{
    return varName;
}

const std::unique_ptr<const Expression> &VarAssignmentStatement::GetVarValue() const
{
    return varValue;
}

void VarAssignmentStatement::SetVarName(std::string varName)
{
    this->varName = varName;
}

void VarAssignmentStatement::SetVarValue(std::unique_ptr<const Expression> &varValue)
{
    this->varValue = std::move(varValue);
}

void VarAssignmentStatement::Print(AstPrinter &printer) const
{
    printer.BeginNode("VarAssignmentStatement");
    printer.Field("Var. Name", varName);
    printer.Field("Var. Value", varValue.get());
    printer.EndNode();
}