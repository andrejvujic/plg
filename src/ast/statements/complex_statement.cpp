#include "ast/statements/complex_statement.hpp"

#include "runtime/interpreter/tree_walk_interpreter.hpp"

ComplexStatement::ComplexStatement() : Statement(AstNodeType::ComplexStatement)
{
}

void ComplexStatement::AddStatement(std::unique_ptr<const Statement> &statement)
{
    body.push_back(std::move(statement));
}

const std::vector<std::unique_ptr<const Statement>> &ComplexStatement::GetStatements() const
{
    return body;
}

void ComplexStatement::Print(AstPrinter &printer) const
{
    printer.BeginNode("ComplexStatement");
    for (auto &statement : body)
        if (statement)
            statement->Print(printer);
    printer.EndNode();
}

std::unique_ptr<RuntimeValue> ComplexStatement::Execute(TreeWalkInterpreter &interpreter) const
{
    interpreter.CreateScope();
    for (auto &statement : body)
        if (statement)
            statement->Execute(interpreter);
    interpreter.DestroyScope();

    return nullptr;
}