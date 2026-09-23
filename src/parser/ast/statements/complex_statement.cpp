#include "parser/ast/statements/complex_statement.hpp"

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

void ComplexStatement::Print(std::ostream &out, size_t depth) const
{
    const std::string indent(depth * 2, ' ');
    out << indent << "[ComplexStatement] {\n";
    for (const auto &statement : body)
        if (statement)
            statement->Print(out, depth + 1);
    out << indent << "}\n";
}
