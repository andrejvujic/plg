#include "ast/ast.hpp"

AstNode::AstNode(AstNodeType t) : type(t)
{
}

AstNodeType AstNode::GetType() const
{
    return type;
}

void AstNode::Print(AstPrinter &printer) const
{
    printer.BeginNode("AstNode");
    printer.Field("Type", std::to_string(static_cast<int>(type)));
    printer.EndNode();
}
