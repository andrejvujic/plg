#ifndef IF_STATEMENT_HPP_INCLUDED
#define IF_STATEMENT_HPP_INCLUDED

#include "parser/ast/statements/statement.hpp"

class IfStatement : public Statement
{
private:
    std::unique_ptr<const Statement> consequent, alternate;
    std::unique_ptr<const Expression> condition;

public:
    IfStatement();

    const std::unique_ptr<const Expression> &GetCondition() const;
    const std::unique_ptr<const Statement> &GetConsequentStatement() const;
    const std::unique_ptr<const Statement> &GetAlternateStatement() const;
    bool HasAlternateStatement() const;

    void SetConsequentStatement(std::unique_ptr<const Statement> &statement);
    void SetAlternateStatement(std::unique_ptr<const Statement> &statement);
    void SetCondition(std::unique_ptr<const Expression> &condition);
    void Print(std::ostream &out, size_t depth = 0) const override;
};

#endif
