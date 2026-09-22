#ifndef STATEMENTS_HPP_INCLUDED
#define STATEMENTS_HPP_INCLUDED

#include "../ast.hpp"
#include "../expressions/expression.hpp"

class Statement : public AstNode
{
private:
public:
    Statement(AstNodeType type);
    virtual ~Statement() = default;
};

class VarDeclarationStatement : public Statement
{
private:
    std::string varName, varType;
    std::unique_ptr<const Expression> initialValue;
    bool hasInitialValue = false;

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

class IfStatement : public Statement
{
private:
public:
    IfStatement();

    void Print(std::ostream &out, size_t depth = 0) const override;
};

class ReturnStatement : public Statement
{
private:
public:
    ReturnStatement();

    void Print(std::ostream &out, size_t depth = 0) const override;
};

class ComplexStatement : public AstNode
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
