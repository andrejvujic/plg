#ifndef FOR_STATEMENT_HPP_INCLUDED
#define FOR_STATEMENT_HPP_INCLUDED

#include "statement.hpp"
#include "ast/expressions/assignment_expression.hpp"
#include "ast/ast_printer.hpp"

class ForStatement : public Statement
{
private:
    std::unique_ptr<const Statement> body;
    std::unique_ptr<const AssignmentExpression> init, update;
    std::unique_ptr<const Expression> condition;

public:
    ForStatement();

    void SetBody(std::unique_ptr<const Statement> &body);
    const std::unique_ptr<const Statement> &GetBody() const;

    void SetInitExpression(std::unique_ptr<const AssignmentExpression> &init);
    const std::unique_ptr<const AssignmentExpression> &GetInitExpression() const;

    void SetUpdateExpression(std::unique_ptr<const AssignmentExpression> &update);
    const std::unique_ptr<const AssignmentExpression> &GetUpdateExpression() const;

    void SetCondition(std::unique_ptr<const Expression> &condition);
    const std::unique_ptr<const Expression> &GetCondition() const;

    void Print(AstPrinter &printer) const override;
};

#endif