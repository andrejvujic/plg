#ifndef VAR_DECLARATION_STATEMENT_HPP_INCLUDED
#define VAR_DECLARATION_STATEMENT_HPP_INCLUDED

#include "ast/statements/statement.hpp"

class VariableDeclaration : public Statement
{
private:
    std::string varName, varType;
    std::unique_ptr<const Expression> initialValue;

public:
    VariableDeclaration();

    const std::string &GetVarName() const;
    const std::string &GetVarType() const;
    const std::unique_ptr<const Expression> &GetInitialValue() const;
    bool HasInitialValue() const;

    void SetVarName(std::string varName);
    void SetVarType(std::string varType);
    void SetInitialVarValue(std::unique_ptr<const Expression> &initialValue);

    void Print(AstPrinter &printer) const override;
};
#endif
