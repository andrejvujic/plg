#include "parser/ast/function.hpp"

Function::Function() : AstNode(AstNodeType::Function)
{
}

std::string_view Function::GetName() const
{
    return name;
}

std::string_view Function::GetReturnType() const
{
    return returnType;
}

void Function::SetName(std::string name)
{
    this->name = name;
}

void Function::SetReturnType(std::string returnType)
{
    this->returnType = returnType;
}

void Function::SetBody(std::unique_ptr<const ComplexStatement> &body)
{
    this->body = std::move(body);
}

void Function::Print(std::ostream &out, size_t depth) const
{
    const std::string indent(depth * 2, ' ');
    const std::string fieldIndent((depth + 1) * 2, ' ');
    out << indent << "[Function] {\n"
        << fieldIndent << "Name -> " << name << '\n'
        << fieldIndent << "Return Type -> " << returnType << '\n';
    if (body)
        body->Print(out, depth + 1);
    out << indent << "}\n";
}
