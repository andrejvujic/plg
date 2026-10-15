#ifndef EXPRESSION_HPP_INCLUDED
#define EXPRESSION_HPP_INCLUDED

#include "ast/ast.hpp"

class Expression : public AstNode
{
public:
    Expression(AstNodeType type, SourceLocation location) : AstNode(type, location) {}

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override = 0;
};

#endif
