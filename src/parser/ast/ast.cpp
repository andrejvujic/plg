#include "parser/ast/ast.hpp"

AstNode::AstNode(AstNodeType t) : type(t)
{
}

AstNodeType AstNode::GetType() const
{
    return type;
}

void AstNode::Print(std::ostream &out, size_t depth) const
{
    const std::string indent(depth * 2, ' ');
    const std::string fieldIndent((depth + 1) * 2, ' ');
    out << indent << "[AstNode] {\n"
        << fieldIndent << "Type -> " << static_cast<int>(type) << '\n'
        << indent << "}\n";
}
