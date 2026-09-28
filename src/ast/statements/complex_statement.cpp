#include "ast/statements/complex_statement.hpp"

ComplexStatement::ComplexStatement() : Statement(AstNodeType::ComplexStatement)
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

void ComplexStatement::Print(AstPrinter &printer) const
{
    printer.BeginNode("ComplexStatement");
    for (const auto &statement : body)
        if (statement)
            statement->Print(printer);
    printer.EndNode();
}
