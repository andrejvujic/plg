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

    void Print(AstPrinter &printer) const override;
};

#endif
