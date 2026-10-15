#include "ast/statements/return_statement.hpp"
#include "runtime/interpreter/tree_walk_interpreter.hpp"
#include "runtime/values/runtime_value.hpp"
#include "runtime/return_signal.hpp"

bool ReturnStatement::HasValue() const
{
    return value != nullptr;
};

const std::unique_ptr<Expression> &ReturnStatement::GetValue() const
{
    return value;
}

void ReturnStatement::Print(AstPrinter &printer) const
{
    printer.BeginNode("ReturnStatement");
    printer.Field("Value", value.get());
    printer.EndNode();
}

std::unique_ptr<RuntimeValue> ReturnStatement::Execute(TreeWalkInterpreter &interpreter) const
{
    if (value)
        throw ReturnSignal(value->Execute(interpreter));

    throw ReturnSignal();
}
