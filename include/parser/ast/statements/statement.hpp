#ifndef STATEMENTS_HPP_INCLUDED
#define STATEMENTS_HPP_INCLUDED

#include "parser/ast/ast.hpp"
#include "parser/ast/expressions/expression.hpp"

class Statement : public AstNode
{
private:
public:
    Statement(AstNodeType type);
    virtual ~Statement() = default;
};

#endif
