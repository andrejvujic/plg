#include "ast/statements/var_declaration_statement.hpp"

VarDeclarationStatement::VarDeclarationStatement() : Statement(AstNodeType::VarDeclartaionStatement)
{
}

const std::string &VarDeclarationStatement::GetVarName() const
{
    return varName;
}

const std::string &VarDeclarationStatement::GetVarType() const
{
    return varType;
}

const std::unique_ptr<const Expression> &VarDeclarationStatement::GetInitialValue() const
{
    return initialValue;
}

void VarDeclarationStatement::SetVarName(std::string varName)
{
    this->varName = varName;
}

void VarDeclarationStatement::SetVarType(std::string varType)
{
    this->varType = varType;
}

void VarDeclarationStatement::SetInitialVarValue(std::unique_ptr<const Expression> &expression)
{
    this->initialValue = std::move(expression);
}

bool VarDeclarationStatement::HasInitialValue() const
{
    return initialValue != nullptr;
}

void VarDeclarationStatement::Print(AstPrinter &printer) const
{
    printer.BeginNode("VarDeclarationStatement");
    printer.Field("Var. Name", varName);
    printer.Field("Var. Type", varType);
    printer.Field("Initial Value", initialValue.get());
    printer.EndNode();
}
