#include "parser/ast/program.hpp"

Program::Program() : AstNode(AstNodeType::Program)
{
}

void Program::AddFunction(std::unique_ptr<const Function> &function)
{
    body.push_back(std::move(function));
}

const std::vector<std::unique_ptr<const Function>> &Program::GetFunctions() const
{
    return body;
}

void Program::Print(AstPrinter &printer) const
{
    printer.BeginNode("Program");
    for (const auto &function : body)
        if (function)
            function->Print(printer);
    printer.EndNode();
}
