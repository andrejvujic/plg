#include "ast/expressions/function_call_expression.hpp"

FunctionCallExpression::FunctionCallExpression() : Expression(AstNodeType::FunctionCallExpression)
{
}

const std::string &FunctionCallExpression::GetFunction() const
{
    return function;
}

void FunctionCallExpression::SetFunction(std::string function)
{
    this->function = function;
}

void FunctionCallExpression::AddArgument(std::unique_ptr<const Expression> &argument)
{
    arguments.push_back(std::move(argument));
}

size_t FunctionCallExpression::GetArgumentCount() const
{
    return arguments.size();
}

const std::vector<std::unique_ptr<const Expression>> &FunctionCallExpression::GetArguments() const
{
    return arguments;
}

void FunctionCallExpression::Print(AstPrinter &printer) const
{
    printer.BeginNode("FunctionCallExpression");
    printer.Field("Function", function);

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