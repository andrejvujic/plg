#include "ast/ast.hpp"

AstNode::AstNode(AstNodeType t) : type(t), location(Location{.column = 0, .line = 0})
{
}

AstNodeType AstNode::GetType() const
{
    return type;
}

Location AstNode::GetLocation() const
{
    return location;
}

void AstNode::SetLocation(Location location)
{
    this->location = location;
}

void AstNode::Print(AstPrinter &printer) const
{
    printer.BeginNode("AstNode");
    printer.Field("Type", std::to_string(static_cast<int>(type)));
    printer.EndNode();
}
