#include "ast/statements/variable_declaration.hpp"

VariableDeclaration::VariableDeclaration() : Statement(AstNodeType::VariableDeclaration)
{
}

const std::string &VariableDeclaration::GetVarName() const
{
    return varName;
}

const std::string &VariableDeclaration::GetVarType() const
{
    return varType;
}

const std::unique_ptr<const Expression> &VariableDeclaration::GetInitialValue() const
{
    return initialValue;
}

void VariableDeclaration::SetVarName(std::string varName)
{
    this->varName = varName;
}

void VariableDeclaration::SetVarType(std::string varType)
{
    this->varType = varType;
}

void VariableDeclaration::SetInitialVarValue(std::unique_ptr<const Expression> &expression)
{
    this->initialValue = std::move(expression);
}

bool VariableDeclaration::HasInitialValue() const
{
    return initialValue != nullptr;
}

void VariableDeclaration::Print(AstPrinter &printer) const
{
    printer.BeginNode("VariableDeclaration");
    printer.Field("Var. Name", varName);
    printer.Field("Var. Type", varType);
    printer.Field("Initial Value", initialValue.get());
    printer.EndNode();
}
