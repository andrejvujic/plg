#ifndef STATEMENTS_HPP_INCLUDED
#define STATEMENTS_HPP_INCLUDED

#include "../ast.hpp"
#include "../expressions/expression.hpp"

class Statement : public AstNode
{
private:
public:
    Statement(AstNodeType type);
    virtual ~Statement() = default;
};

#endif
