#ifndef AST_HPP_INCLUDED
#define AST_HPP_INCLUDED

#include <vector>
#include <memory>
#include <string>
#include <string_view>
#include <iostream>

enum class AstNodeType
{
    Program,
    Function,
    ComplexStatement,
    IfStatement,
    ReturnStatement,
    VarDeclartaionStatement,
    NumLiteralExpression,
    IdExpression,
};

class AstNode
{
private:
    AstNodeType type;

public:
    AstNode(AstNodeType type);
    AstNodeType GetType() const;
    virtual void Print(std::ostream &out, size_t depth = 0) const;
    virtual ~AstNode() = default;
};
#endif
