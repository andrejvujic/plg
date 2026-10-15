#ifndef AST_NODE_HPP_INCLUDED
#define AST_NODE_HPP_INCLUDED

#include <iostream>

enum class AstNodeType
{
    Program,
    FunctionDeclaration,
    FunctionParameter,
    ComplexStatement,
    IfStatement,
    ReturnStatement,
    VariableDeclaration,
    AssignmentExpression,
    ForStatement,
    ExpressionStatement,
    NumLiteralExpression,
    BoolLiteralExpression,
    IdExpression,
    BinaryExpression,
    CallExpression,
};

std::ostream &operator<<(std::ostream &out, const AstNodeType &type);

#endif