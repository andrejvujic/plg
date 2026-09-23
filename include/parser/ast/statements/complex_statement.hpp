#ifndef COMPLEX_STATEMENT_HPP_INCLUDED
#define COMPLEX_STATEMENT_HPP_INCLUDED

#include "parser/ast/statements/statement.hpp"

class ComplexStatement : public Statement
{
private:
    std::vector<std::unique_ptr<const Statement>> body;

public:
    ComplexStatement();

    const std::vector<std::unique_ptr<const Statement>> &GetStatements();
    void AddStatement(std::unique_ptr<const Statement> &statement);

    void Print(std::ostream &out, size_t depth = 0) const override;
};

#endif
