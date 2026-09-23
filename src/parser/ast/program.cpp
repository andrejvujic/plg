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

void Program::Print(std::ostream &out, size_t depth) const
{
    const std::string indent(depth * 2, ' ');
    out << indent << "[Program] {\n";
    for (const auto &function : body)
    {
        function->Print(out, depth + 1);
    }
    out << indent << "}\n";
}
