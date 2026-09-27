#ifndef FUNCTIONS_HPP_INCLUDED
#define FUNCTIONS_HPP_INCLUDED

#include "parser/ast/ast.hpp"
#include "parser/ast/statements/statement.hpp"
#include "parser/ast/statements/complex_statement.hpp"
#include "parser/ast/function/function_parameter.hpp"

class Function : public AstNode
{
private:
    std::string name, returnType;
    std::vector<std::unique_ptr<const FunctionParameter>> parameters;

public:
    Function();

    std::string_view GetName() const;
    std::string_view GetReturnType() const;
    std::unique_ptr<const ComplexStatement> body;

    void SetName(std::string name);
    void SetReturnType(std::string returnType);
    void SetBody(std::unique_ptr<const ComplexStatement> &body);

    void AddParameter(std::unique_ptr<const FunctionParameter> &parameter);
    size_t GetParameterCount() const;

    void Print(AstPrinter &printer) const override;
};

#endif
