#include "ast/expressions/call_expression.hpp"
#include "ast/expressions/id_expression.hpp"

#include "runtime/interpreter/tree_walk_interpreter.hpp"
#include "runtime/return_signal.hpp"
#include "runtime/bindings/function_binding.hpp"
#include "runtime/bindings/variable_binding.hpp"
#include "runtime/scoped_environment.hpp"

void CallExpression::AddArgument(std::unique_ptr<Expression> argument)
{
    arguments.push_back(std::move(argument));
}

const std::unique_ptr<Expression> &CallExpression::GetCallee() const
{
    return callee;
}

size_t CallExpression::GetArgumentCount() const
{
    return arguments.size();
}

const std::vector<std::unique_ptr<Expression>> &CallExpression::GetArguments() const
{
    return arguments;
}

void CallExpression::Print(AstPrinter &printer) const
{
    printer.BeginNode("CallExpression");
    printer.Field("Callee", callee.get());

    printer.Field("Argument Count", arguments.size());
    if (!arguments.empty())
    {
        printer.BeginNode("Arguments");
        for (auto &argument : arguments)
            argument->Print(printer);
        printer.EndNode();
    }

    printer.EndNode();
}

std::unique_ptr<RuntimeValue> CallExpression::Execute(TreeWalkInterpreter &interpreter) const
{
    if (callee->GetType() == AstNodeType::IdExpression)
        return ExecuteNonMemberFunction(interpreter);

    std::cerr << "[CallExpression] ERROR: Execution not supported for " << callee->GetType() << std::endl;
    exit(EXIT_FAILURE);
}

std::unique_ptr<RuntimeValue> CallExpression::ExecuteNonMemberFunction(TreeWalkInterpreter &interpreter) const
{
    ScopedEnvironment environmentGuard(interpreter);
    auto environment = interpreter.GetCurrentEnvironment();

    auto c = dynamic_cast<IdExpression *>(callee.get());
    if (!environment->BindingExists(c->GetValue(), BindingKind::Function))
        std::exit(EXIT_FAILURE);

    auto &binding = environment->GetBinding(c->GetValue(), BindingKind::Function);
    auto function = dynamic_cast<FunctionBinding *>(binding.get());
    auto &declaration = function->GetDeclaration();
    auto &body = declaration.GetBody();

    DeclareArgumentBindings(interpreter, declaration.GetParameters());
    try
    {
        body->Execute(interpreter);
    }
    catch (const ReturnSignal &returnSignal)
    {
        auto &returnValue = returnSignal.GetValue();
        return returnValue->Clone();
    }

    std::cerr << "[CallExpression] ERROR: Non-void function didn't return a value" << std::endl;
    std::exit(EXIT_FAILURE);
}

void CallExpression::DeclareArgumentBindings(TreeWalkInterpreter &interpreter, const std::vector<std::unique_ptr<FunctionParameter>> &parameters) const
{
    auto environment = interpreter.GetCurrentEnvironment();

    if (parameters.size() != arguments.size())
        std::exit(EXIT_FAILURE);

    for (size_t index = 0; index < arguments.size(); ++index)
    {
        auto &parameter = parameters[index];
        auto &argument = arguments[index];
        auto argumentValue = argument->Execute(interpreter);
        auto binding = std::make_unique<VariableBinding>(
            parameter->GetVarName(),
            parameter->GetVarType(),
            std::move(argumentValue));
        environment->DeclareBinding(std::move(binding));
    }
}