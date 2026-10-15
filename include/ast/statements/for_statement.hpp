#ifndef FOR_STATEMENT_HPP_INCLUDED
#define FOR_STATEMENT_HPP_INCLUDED

#include "statement.hpp"
#include "ast/ast_printer.hpp"

class ForStatement : public Statement
{
private:
    std::unique_ptr<Statement> body;
    std::unique_ptr<Expression> init, update, condition;

public:
    ForStatement(SourceLocation location,
                 std::unique_ptr<Expression> init,
                 std::unique_ptr<Expression> condition,
                 std::unique_ptr<Expression> update,
                 std::unique_ptr<Statement> body)
        : Statement(AstNodeType::ForStatement, location),
          body(std::move(body)),
          init(std::move(init)),
          update(std::move(update)),
          condition(std::move(condition)) {}

    const std::unique_ptr<Statement> &GetBody() const;

    const std::unique_ptr<Expression> &GetInitExpression() const;

    const std::unique_ptr<Expression> &GetUpdateExpression() const;

    const std::unique_ptr<Expression> &GetCondition() const;

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;
};

#endif
