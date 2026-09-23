#ifndef FUNCTIONS_HPP_INCLUDED
#define FUNCTIONS_HPP_INCLUDED

#include "parser/ast/ast.hpp"
#include "parser/ast/statements/statement.hpp"
#include "parser/ast/statements/complex_statement.hpp"

class Function : public AstNode
{
private:
    std::string name, returnType;

public:
    Function();

    std::string_view GetName() const;
    std::string_view GetReturnType() const;
    std::unique_ptr<const ComplexStatement> body;

    void SetName(std::string name);
    void SetReturnType(std::string returnType);
    void SetBody(std::unique_ptr<const ComplexStatement> &body);

    void Print(std::ostream &out, size_t depth = 0) const override;
};

#endif
