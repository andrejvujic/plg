#include "ast/statements/functions/function_declaration.hpp"

#include "semantic_analysis/symbols/function_symbol.hpp"
#include "runtime/interpreter/tree_walk_interpreter.hpp"

FunctionDeclaration::FunctionDeclaration() : Statement(AstNodeType::FunctionDeclaration)
{
}

const std::string &FunctionDeclaration::GetName() const
{
    return name;
}

const std::string &FunctionDeclaration::GetReturnType() const
{
    return returnType;
}

const std::unique_ptr<ComplexStatement> &FunctionDeclaration::GetBody() const
{
    return body;
}

void FunctionDeclaration::SetName(std::string name)
{
    this->name = name;
}

void FunctionDeclaration::SetReturnType(std::string returnType)
{
    this->returnType = returnType;
}

void FunctionDeclaration::SetBody(std::unique_ptr<ComplexStatement> &body)
{
    this->body = std::move(body);
}

void FunctionDeclaration::AddParameter(std::unique_ptr<FunctionParameter> &parameter)
{
    parameters.push_back(std::move(parameter));
}

const std::vector<std::unique_ptr<FunctionParameter>> &FunctionDeclaration::GetParameters() const
{
    return parameters;
}

size_t FunctionDeclaration::GetParameterCount() const
{
    return parameters.size();
}

void FunctionDeclaration::Print(AstPrinter &printer) const
{
    printer.BeginNode("FunctionDeclaration");
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

std::unique_ptr<RuntimeValue> FunctionDeclaration::Execute(TreeWalkInterpreter &interpreter) const
{
    FunctionSymbol function(name, returnType);
    function.SetFunctionDeclaration(this);
    for (auto &parameter : GetParameters())
        function.AddParameterType(parameter->GetVarType());

    std::unique_ptr<Symbol> symbol = std::make_unique<FunctionSymbol>(function);
    auto scope = interpreter.GetCurrentScope();
    scope->DeclareSymbol(symbol);

    return nullptr;
}
