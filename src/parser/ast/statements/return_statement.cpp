#include "parser/ast/statements/return_statement.hpp"

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

void ReturnStatement::Print(std::ostream &out, size_t depth) const
{
    auto indent = std::string(depth * 2, ' ');
    out << indent << "[ReturnStatement] {\n";
    if (value)
        value->Print(out, depth + 1);
    out << indent << "}\n";
};
