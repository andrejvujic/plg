#ifndef VAR_DECLARATION_STATEMENT_HPP_INCLUDED
#define VAR_DECLARATION_STATEMENT_HPP_INCLUDED

#include "parser/ast/statements/statement.hpp"

class VarDeclarationStatement : public Statement
{
private:
    std::string varName, varType;
    std::unique_ptr<const Expression> initialValue;

public:
    VarDeclarationStatement();

    std::string_view GetVarName() const;
    std::string_view GetVarType() const;
    const std::unique_ptr<const Expression> &GetInitialValue() const;
    bool HasInitialValue() const;

    void SetVarName(std::string varName);
    void SetVarType(std::string varType);
    void SetInitialValue(std::unique_ptr<const Expression> &initialValue);

    void Print(std::ostream &out, size_t depth = 0) const override;
};
#endif
