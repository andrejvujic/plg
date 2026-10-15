#include "ast/program.hpp"
#include "runtime/interpreter/tree_walk_interpreter.hpp"

Program::Program() : AstNode(AstNodeType::Program)
{
}

void Program::AddStatement(std::unique_ptr<Statement> &statement)
{
    statements.push_back(std::move(statement));
}

const std::vector<std::unique_ptr<Statement>> &Program::GetStatements() const
{
    return statements;
}

std::unique_ptr<RuntimeValue> Program::Execute(TreeWalkInterpreter &interpreter) const
{
    for (const auto &statement : statements)
        statement->Execute(interpreter);
    return nullptr;
}

void Program::Print(AstPrinter &printer) const
{
    printer.BeginNode("Program");
    for (const auto &statement : statements)
        if (statement)
            statement->Print(printer);
    printer.EndNode();
}
