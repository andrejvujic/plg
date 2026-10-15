#include "runtime/bindings/binding.hpp"

const std::string &Binding::GetName() const
{
    return name;
}

const std::string &Binding::GetType() const
{
    return type;
}

BindingKind Binding::GetKind() const
{
    return kind;
}

bool Binding::IsOfKind(BindingKind kind) const
{
    return this->kind == kind;
}
