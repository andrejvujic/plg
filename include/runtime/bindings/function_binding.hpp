#ifndef FUNCTION_BINDING_HPP_INCLUDED
#define FUNCTION_BINDING_HPP_INCLUDED

#include <string_view>

#include "ast/statements/functions/function_declaration.hpp"
#include "runtime/bindings/binding.hpp"

class FunctionBinding : public Binding
{
private:
    const FunctionDeclaration *declaration;

public:
    explicit FunctionBinding(const FunctionDeclaration &declaration)
        : Binding(declaration.GetName(), declaration.GetReturnType(), BindingKind::Function),
          declaration(&declaration) {}

    const FunctionDeclaration &GetDeclaration() const;
    void Print(std::ostream &out) const override;
};

#endif
