#include "parser/ast/expressions/function_call_expression.hpp"

FunctionCallExpression::FunctionCallExpression() : Expression(AstNodeType::FunctionCallExpression)
{
}

std::string_view FunctionCallExpression::GetFunction() const
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