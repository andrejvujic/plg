#ifndef EXPRESSION_HPP_INCLUDED
#define EXPRESSION_HPP_INCLUDED

#include "ast/ast.hpp"

class Expression : public AstNode
{
public:
    Expression(AstNodeType type);
};

#endif
