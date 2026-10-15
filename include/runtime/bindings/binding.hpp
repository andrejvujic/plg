#ifndef RUNTIME_BINDING_HPP_INCLUDED
#define RUNTIME_BINDING_HPP_INCLUDED

#include <iostream>
#include <string>
#include <string_view>

enum class BindingKind
{
    Function,
    Variable,
};

class Binding
{
private:
    std::string name, type;
    BindingKind kind;

public:
    Binding(std::string_view name, std::string_view type, BindingKind kind)
        : name(name), type(type), kind(kind) {}
    virtual ~Binding() = default;

    const std::string &GetName() const;
    const std::string &GetType() const;
    BindingKind GetKind() const;
    bool IsOfKind(BindingKind kind) const;

    virtual void Print(std::ostream &out) const = 0;
};

#endif
