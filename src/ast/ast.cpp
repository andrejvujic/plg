#include "ast/ast.hpp"
#include "runtime/interpreter/tree_walk_interpreter.hpp"

AstNodeType AstNode::GetType() const
{
    return type;
}

SourceLocation AstNode::GetLocation() const
{
    return location;
}
