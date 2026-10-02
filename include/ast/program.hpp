#ifndef PROGRAM_HPP_INCLUDED
#define PROGRAM_HPP_INCLUDED

#include <vector>

#include "ast/ast.hpp"
#include "ast/statements/statement.hpp"

class Program : public AstNode
{
private:
    std::vector<std::unique_ptr<const Statement>> statements;

public:
    Program();

    void AddStatement(std::unique_ptr<const Statement> &statement);
    const std::vector<std::unique_ptr<const Statement>> &GetStatements() const;
    void Print(AstPrinter &printer) const override;
};

#endif
