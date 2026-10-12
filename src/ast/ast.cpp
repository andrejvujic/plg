#include "ast/ast.hpp"
#include "runtime/interpreter/tree_walk_interpreter.hpp"

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
