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
    ForStatement();

    void SetBody(std::unique_ptr<Statement> &body);
    const std::unique_ptr<Statement> &GetBody() const;

    void SetInitExpression(std::unique_ptr<Expression> &init);
    const std::unique_ptr<Expression> &GetInitExpression() const;

    void SetUpdateExpression(std::unique_ptr<Expression> &update);
    const std::unique_ptr<Expression> &GetUpdateExpression() const;

    void SetCondition(std::unique_ptr<Expression> &condition);
    const std::unique_ptr<Expression> &GetCondition() const;

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;
};

#endif
