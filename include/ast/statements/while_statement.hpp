#ifndef WHILE_STATEMENT_HPP_INCLUDED
#define WHILE_STATEMENT_HPP_INCLUDED

#include "ast/statements/statement.hpp"
#include "runtime/values/runtime_value.hpp"
#include "runtime/interpreter/tree_walk_interpreter.hpp"

class WhileStatement : public Statement
{
private:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> body;

public:
    WhileStatement(SourceLocation location, std::unique_ptr<Expression> condition, std::unique_ptr<Statement> body)
        : Statement(AstNodeType::WhileStatement, location),
          condition(std::move(condition)), body(std::move(body)) {}

    const std::unique_ptr<Expression> &GetCondition() const;
    const std::unique_ptr<Statement> &GetBody() const;

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;
};

#endif