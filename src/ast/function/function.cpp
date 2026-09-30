#include "ast/function/function.hpp"

Function::Function() : AstNode(AstNodeType::Function)
{
}

const std::string &Function::GetName() const
{
    return name;
}

const std::string &Function::GetReturnType() const
{
    return returnType;
}

const std::unique_ptr<const ComplexStatement> &Function::GetBody() const
{
    return body;
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

const std::vector<std::unique_ptr<const FunctionParameter>> &Function::GetParameters() const
{
    return parameters;
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
