#ifndef AST_HPP_INCLUDED
#define AST_HPP_INCLUDED

#include <vector>
#include <memory>
#include <string>
#include <string_view>
#include <iostream>

#include "ast_printer.hpp"
#include "shared/location.hpp"
#include "runtime/runtime_value.hpp"

class TreeWalkInterpreter;

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
    virtual ~AstNode() = default;

    AstNodeType GetType() const;

    Location GetLocation() const;
    void SetLocation(Location location);

    virtual std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const = 0;

    virtual void Print(AstPrinter &printer) const = 0;
};
#endif
