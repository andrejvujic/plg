#include "parser/ast/statements/return_statement.hpp"

ReturnStatement::ReturnStatement() : Statement(AstNodeType::ReturnStatement)
{
}

void ReturnStatement::Print(std::ostream &out, size_t depth) const
{
    out << std::string(depth * 2, ' ') << "[ReturnStatement] {}\n";
}
