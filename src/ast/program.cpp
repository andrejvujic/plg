#include "ast/program.hpp"

Program::Program() : AstNode(AstNodeType::Program)
{
}

void Program::AddStatement(std::unique_ptr<const Statement> &statement)
{
    statements.push_back(std::move(statement));
}

const std::vector<std::unique_ptr<const Statement>> &Program::GetStatements() const
{
    return statements;
}

void Program::Print(AstPrinter &printer) const
{
    printer.BeginNode("Program");
    for (const auto &statement : statements)
        if (statement)
            statement->Print(printer);
    printer.EndNode();
}
