#include "parser/ast/statements/if_statement.hpp"

IfStatement::IfStatement() : Statement(AstNodeType::IfStatement)
{
}

void IfStatement::Print(std::ostream &out, size_t depth) const
{
    out << std::string(depth * 2, ' ') << "[IfStatement] {}\n";
}
