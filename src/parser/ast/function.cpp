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

void Function::Print(AstPrinter &printer) const
{
    printer.BeginNode("Function");
    printer.Field("Name", name);
    printer.Field("Return Type", returnType);
    printer.Field("Body", body.get());
    printer.EndNode();
}
