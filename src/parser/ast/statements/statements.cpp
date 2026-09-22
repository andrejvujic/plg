#include "statements.hpp"
#include "../expressions/expression.hpp"

Statement::Statement(AstNodeType type) : AstNode(type)
{
}

VarDeclarationStatement::VarDeclarationStatement() : Statement(AstNodeType::VarDeclartaionStatement)
{
}

std::string_view VarDeclarationStatement::GetVarName() const
{
    return varName;
}

std::string_view VarDeclarationStatement::GetVarType() const
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

void VarDeclarationStatement::SetInitialValue(std::unique_ptr<const Expression> &expression)
{
    this->initialValue = std::move(expression);
    this->hasInitialValue = true;
}

void VarDeclarationStatement::Print(std::ostream &out, size_t depth) const
{
    const std::string indent(depth * 2, ' ');
    const std::string fieldIndent((depth + 1) * 2, ' ');
    out << indent << "[VarDeclarationStatement] {\n"
        << fieldIndent << "Var. Name -> " << varName << '\n'
        << fieldIndent << "Var. Type -> " << varType << '\n';
    if (initialValue)
        initialValue->Print(out, depth + 1);
    out << indent << "}\n";
}

ComplexStatement::ComplexStatement() : AstNode(AstNodeType::ComplexStatement)
{
}

void ComplexStatement::AddStatement(std::unique_ptr<const Statement> &statement)
{
    body.push_back(std::move(statement));
}

const std::vector<std::unique_ptr<const Statement>> &ComplexStatement::GetStatements()
{
    return body;
}

IfStatement::IfStatement() : Statement(AstNodeType::IfStatement)
{
}

ReturnStatement::ReturnStatement() : Statement(AstNodeType::ReturnStatement)
{
}

void ComplexStatement::Print(std::ostream &out, size_t depth) const
{
    const std::string indent(depth * 2, ' ');
    out << indent << "[ComplexStatement] {\n";
    for (const auto &statement : body)
        if (statement)
            statement->Print(out, depth + 1);
    out << indent << "}\n";
}

void IfStatement::Print(std::ostream &out, size_t depth) const
{
    out << std::string(depth * 2, ' ') << "[IfStatement] {}\n";
}

void ReturnStatement::Print(std::ostream &out, size_t depth) const
{
    out << std::string(depth * 2, ' ') << "[ReturnStatement] {}\n";
}
