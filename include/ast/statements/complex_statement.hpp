#ifndef COMPLEX_STATEMENT_HPP_INCLUDED
#define COMPLEX_STATEMENT_HPP_INCLUDED

#include "ast/statements/statement.hpp"

class ComplexStatement : public Statement
{
private:
    std::vector<std::unique_ptr<Statement>> body;

public:
    explicit ComplexStatement(SourceLocation location)
        : Statement(AstNodeType::ComplexStatement, location) {}

    const std::vector<std::unique_ptr<Statement>> &GetStatements() const;
    void AddStatement(std::unique_ptr<Statement> statement);

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;
};

#endif
