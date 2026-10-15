#ifndef FUNCTION_HPP_INCLUDED
#define FUNCTION_HPP_INCLUDED

#include "ast/ast.hpp"
#include "ast/statements/statement.hpp"
#include "ast/statements/complex_statement.hpp"
#include "ast/statements/functions/function_parameter.hpp"

class FunctionDeclaration : public Statement
{
private:
    std::string name, returnType;
    std::vector<std::unique_ptr<FunctionParameter>> parameters;
    std::unique_ptr<ComplexStatement> body;

public:
    FunctionDeclaration(SourceLocation location,
                        std::string name,
                        std::string returnType,
                        std::unique_ptr<ComplexStatement> body)
        : Statement(AstNodeType::FunctionDeclaration, location),
          name(std::move(name)),
          returnType(std::move(returnType)),
          body(std::move(body)) {}

    const std::string &GetName() const;
    const std::string &GetReturnType() const;
    const std::unique_ptr<ComplexStatement> &GetBody() const;

    void AddParameter(std::unique_ptr<FunctionParameter> parameter);
    const std::vector<std::unique_ptr<FunctionParameter>> &GetParameters() const;
    size_t GetParameterCount() const;

    std::unique_ptr<RuntimeValue> Execute(TreeWalkInterpreter &interpreter) const override;

    void Print(AstPrinter &printer) const override;
};

#endif
