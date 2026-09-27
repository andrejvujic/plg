#include "parser/ast/function/function.hpp"

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

void Function::AddParameter(std::unique_ptr<const FunctionParameter> &parameter)
{
    parameters.push_back(std::move(parameter));
}

size_t Function::GetParameterCount() const
{
    return parameters.size();
}

void Function::Print(AstPrinter &printer) const
{
    printer.BeginNode("Function");
    printer.Field("Name", name);
    printer.Field("Return Type", returnType);
    printer.Field("Body", body.get());

    printer.Field("Parameter Count", static_cast<int>(parameters.size()));
    if (!parameters.empty())
    {
        printer.BeginNode("Parameters");
        for (auto &parameter : parameters)
            parameter->Print(printer);
        printer.EndNode();
    }

    printer.EndNode();
}
