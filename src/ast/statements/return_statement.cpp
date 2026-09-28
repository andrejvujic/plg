#include "ast/statements/return_statement.hpp"

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
