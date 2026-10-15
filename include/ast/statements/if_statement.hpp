#ifndef IF_STATEMENT_HPP_INCLUDED
#define IF_STATEMENT_HPP_INCLUDED

#include "ast/statements/statement.hpp"

class IfStatement : public Statement
{
private:
    std::unique_ptr<Statement> consequent, alternate;
    std::unique_ptr<Expression> condition;

public:
    IfStatement();

    const std::unique_ptr<Expression> &GetCondition() const;
    const std::unique_ptr<Statement> &GetConsequentStatement() const;
    const std::unique_ptr<Statement> &GetAlternateStatement() const;
    bool HasAlternateStatement() const;

    void SetConsequentStatement(std::unique_ptr<Statement> &statement);
    void SetAlternateStatement(std::unique_ptr<Statement> &statement);
    void SetCondition(std::unique_ptr<Expression> &condition);

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;
};

#endif
