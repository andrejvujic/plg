#include "ast/expressions/function_call_expression.hpp"
#include "runtime/interpreter/tree_walk_interpreter.hpp"
#include "runtime/return_signal.hpp"
#include "symbols/function_symbol.hpp"
#include "symbols/variable_symbol.hpp"
#include "runtime/scoped_new_scope.hpp"

void FunctionCallExpression::AddArgument(std::unique_ptr<Expression> argument)
{
    arguments.push_back(std::move(argument));
}

const std::string &FunctionCallExpression::GetCallee() const
{
    return callee;
}

size_t FunctionCallExpression::GetArgumentCount() const
{
    return arguments.size();
}

const std::vector<std::unique_ptr<Expression>> &FunctionCallExpression::GetArguments() const
{
    return arguments;
}

void FunctionCallExpression::Print(AstPrinter &printer) const
{
    printer.BeginNode("FunctionCallExpression");
    printer.Field("Callee", callee);

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

std::unique_ptr<RuntimeValue> FunctionCallExpression::Execute(TreeWalkInterpreter &interpreter) const
{
    ScopedNewScope scopeGuard(interpreter);
    auto scope = interpreter.GetCurrentScope();

    if (!scope->SymbolExists(callee, SymbolKind::Function))
        std::exit(EXIT_FAILURE);

    auto &symbol = scope->GetSymbol(callee, SymbolKind::Function);
    auto function = dynamic_cast<FunctionSymbol *>(symbol.get());

    try
    {
        auto declaration = function->GetFunctionDeclaration();
        auto &parameters = declaration->GetParameters();

        auto parameterIterartor = parameters.begin();
        auto argumentIterator = arguments.begin();
        for (; parameterIterartor != parameters.end() && argumentIterator != arguments.end();
             ++parameterIterartor, ++argumentIterator)
        {
            auto &argument = *argumentIterator;
            auto &parameter = *parameterIterartor;

            auto value = argument->Execute(interpreter);
            std::unique_ptr<Symbol> symbol = std::make_unique<VariableSymbol>(
                parameter->GetVarName(),
                parameter->GetVarType(),
                std::move(value));
            scope->DeclareSymbol(std::move(symbol));
        }

        auto &body = declaration->GetBody();
        body->Execute(interpreter);
    }
    catch (const ReturnSignal &returnSignal)
    {
        interpreter.DestroyScope();
        auto &returnValue = returnSignal.GetValue();
        return returnValue->Clone();
    }

    std::cerr << "[FunctionCallExpression] ERROR: Non-void function didn't return a value" << std::endl;
    std::exit(EXIT_FAILURE);
}
