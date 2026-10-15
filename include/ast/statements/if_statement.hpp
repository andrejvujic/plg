#ifndef IF_STATEMENT_HPP_INCLUDED
#define IF_STATEMENT_HPP_INCLUDED

#include "ast/statements/statement.hpp"

class IfStatement : public Statement
{
private:
    std::unique_ptr<Statement> consequent, alternate;
    std::unique_ptr<Expression> condition;

public:
    IfStatement(SourceLocation location,
                std::unique_ptr<Expression> condition,
                std::unique_ptr<Statement> consequent)
        : Statement(AstNodeType::IfStatement, location),
          consequent(std::move(consequent)),
          condition(std::move(condition)) {}

    IfStatement(SourceLocation location,
                std::unique_ptr<Expression> condition,
                std::unique_ptr<Statement> consequent,
                std::unique_ptr<Statement> alternate)
        : Statement(AstNodeType::IfStatement, location),
          consequent(std::move(consequent)),
          alternate(std::move(alternate)),
          condition(std::move(condition)) {}

    const std::unique_ptr<Expression> &GetCondition() const;
    const std::unique_ptr<Statement> &GetConsequentStatement() const;
    const std::unique_ptr<Statement> &GetAlternateStatement() const;
    bool HasAlternateStatement() const;

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;
};

#endif
