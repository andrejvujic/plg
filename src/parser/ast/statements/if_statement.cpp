#include "parser/ast/statements/if_statement.hpp"

IfStatement::IfStatement() : Statement(AstNodeType::IfStatement)
{
}

const std::unique_ptr<const Expression> &IfStatement::GetCondition() const
{
    return condition;
}

const std::unique_ptr<const Statement> &IfStatement::GetConsequentStatement() const
{
    return consequent;
}

const std::unique_ptr<const Statement> &IfStatement::GetAlternateStatement() const
{
    return alternate;
}

bool IfStatement::HasAlternateStatement() const
{
    return alternate != nullptr;
}

void IfStatement::SetConsequentStatement(std::unique_ptr<const Statement> &statement)
{
    this->consequent = std::move(statement);
}

void IfStatement::SetAlternateStatement(std::unique_ptr<const Statement> &statement)
{
    this->alternate = std::move(statement);
}

void IfStatement::SetCondition(std::unique_ptr<const Expression> &condition)
{
    this->condition = std::move(condition);
}

void IfStatement::Print(std::ostream &out, size_t depth) const
{
    const auto indent = std::string(depth * 2, ' ');
    out << indent << "[IfStatement] {\n";
    condition->Print(out, depth + 1);
    consequent->Print(out, depth + 1);
    if (alternate)
        alternate->Print(out, depth + 1);
    out << indent << "}\n";
}
