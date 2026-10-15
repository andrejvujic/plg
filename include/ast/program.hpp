#ifndef PROGRAM_HPP_INCLUDED
#define PROGRAM_HPP_INCLUDED

#include <vector>

#include "ast/ast.hpp"
#include "ast/statements/statement.hpp"

class Program : public AstNode
{
private:
    std::vector<std::unique_ptr<Statement>> statements;

public:
    Program();

    void AddStatement(std::unique_ptr<Statement> &statement);
    const std::vector<std::unique_ptr<Statement>> &GetStatements() const;

    virtual std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;
};

#endif
