#ifndef VAR_DECLARATION_STATEMENT_HPP_INCLUDED
#define VAR_DECLARATION_STATEMENT_HPP_INCLUDED

#include "ast/statements/statement.hpp"

class VariableDeclaration : public Statement
{
private:
    std::string varName, varType;
    std::unique_ptr<Expression> initialValue;

public:
    VariableDeclaration(SourceLocation location,
                        std::string varName,
                        std::string varType)
        : Statement(AstNodeType::VariableDeclaration, location),
          varName(std::move(varName)),
          varType(std::move(varType)) {}

    VariableDeclaration(SourceLocation location,
                        std::string varName,
                        std::string varType,
                        std::unique_ptr<Expression> initialValue)
        : Statement(AstNodeType::VariableDeclaration, location),
          varName(std::move(varName)),
          varType(std::move(varType)),
          initialValue(std::move(initialValue)) {}

    const std::string &GetVarName() const;
    const std::string &GetVarType() const;
    const std::unique_ptr<Expression> &GetInitialValue() const;
    bool HasInitialValue() const;

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;
};
#endif
