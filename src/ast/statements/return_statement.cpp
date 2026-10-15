#include "ast/statements/return_statement.hpp"
#include "runtime/interpreter/tree_walk_interpreter.hpp"
#include "runtime/runtime_value.hpp"
#include "runtime/return_signal.hpp"

ReturnStatement::ReturnStatement() : Statement(AstNodeType::ReturnStatement)
{
}

bool ReturnStatement::HasValue() const
{
    return value != nullptr;
};

const std::unique_ptr<const Expression> &ReturnStatement::GetValue() const
{
    return value;
}

void ReturnStatement::SetValue(std::unique_ptr<const Expression> &value)
{
    this->value = std::move(value);
}

void ReturnStatement::Print(AstPrinter &printer) const
{
    printer.BeginNode("ReturnStatement");
    printer.Field("Value", value.get());
    printer.EndNode();
}

std::unique_ptr<RuntimeValue> ReturnStatement::Execute(TreeWalkInterpreter &interpreter) const
{
    throw ReturnSignal(value ? value->Execute(interpreter) : nullptr);
}