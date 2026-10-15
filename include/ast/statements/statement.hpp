#ifndef STATEMENTS_HPP_INCLUDED
#define STATEMENTS_HPP_INCLUDED

#include "ast/ast.hpp"
#include "ast/expressions/expression.hpp"

class Statement : public AstNode
{
private:
public:
    Statement(AstNodeType type, SourceLocation location) : AstNode(type, location) {}
    virtual ~Statement() = default;
};

#endif
