#ifndef VAR_ASSIGNMENT_STATEMENT_HPP_INCLUDED
#define VAR_ASSIGNMENT_STATEMENT_HPP_INCLUDED

#include "statement.hpp"

class VarAssignmentStatement : public Statement
{
private:
    std::string varName;
    std::unique_ptr<const Expression> varValue;

public:
    VarAssignmentStatement();

    std::string_view GetVarName() const;
    const std::unique_ptr<const Expression> &GetVarValue() const;
    void SetVarName(std::string varName);
    void SetVarValue(std::unique_ptr<const Expression> &varValue);

    void Print(AstPrinter &printer) const;
};

#endif