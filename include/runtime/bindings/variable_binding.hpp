#ifndef VARIABLE_BINDING_HPP_INCLUDED
#define VARIABLE_BINDING_HPP_INCLUDED

#include <memory>
#include <string>

#include "runtime/bindings/binding.hpp"
#include "runtime/values/runtime_value.hpp"

class VariableBinding : public Binding
{
private:
    std::unique_ptr<RuntimeValue> value;

public:
    VariableBinding(std::string name, std::string type, std::unique_ptr<RuntimeValue> value)
        : Binding(name, type, BindingKind::Variable), value(std::move(value)) {}

    std::unique_ptr<RuntimeValue> &GetValue();
    void SetValue(std::unique_ptr<RuntimeValue> value);
    void Print(std::ostream &out) const override;
};

#endif
