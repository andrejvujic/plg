#ifndef AST_HPP_INCLUDED
#define AST_HPP_INCLUDED

#include <vector>
#include <memory>
#include <string>
#include <string_view>
#include <iostream>

#include "ast_printer.hpp"
#include "shared/location.hpp"

enum class AstNodeType
{
    Program,
    Function,
    FunctionParameter,
    ComplexStatement,
    IfStatement,
    ReturnStatement,
    VarDeclartaionStatement,
    AssignmentExpression,
    ForStatement,
    ExpressionStatement,
    NumLiteralExpression,
    IdExpression,
    BinaryExpression,
    FunctionCallExpression,
    FunctionCallArguments,
};

class AstNode
{
private:
    AstNodeType type;
    Location location;

public:
    AstNode(AstNodeType type);
    AstNodeType GetType() const;

    Location GetLocation() const;
    void SetLocation(Location location);

    virtual void Print(AstPrinter &printer) const;
    virtual ~AstNode() = default;
};
#endif
