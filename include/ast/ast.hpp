#ifndef AST_HPP_INCLUDED
#define AST_HPP_INCLUDED

#include <vector>
#include <memory>
#include <string>
#include <string_view>
#include <iostream>
#include <utility>

#include "ast/ast_node_type.hpp"
#include "ast/ast_printer.hpp"

#include "shared/source_location.hpp"
#include "runtime/values/runtime_value.hpp"

class TreeWalkInterpreter;

class AstNode
{
private:
    AstNodeType type;
    SourceLocation location;

public:
    AstNode(AstNodeType type, SourceLocation location) : type(type), location(location) {}
    virtual ~AstNode() = default;

    AstNodeType GetType() const;

    SourceLocation GetLocation() const;

    virtual std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const = 0;

    virtual void Print(AstPrinter &printer) const = 0;
};
#endif
