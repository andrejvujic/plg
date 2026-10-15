#ifndef ENVIRONMENT_HPP_INCLUDED
#define ENVIRONMENT_HPP_INCLUDED

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "runtime/bindings/binding.hpp"

class Environment
{
private:
    std::shared_ptr<Environment> parent;
    std::unordered_map<std::string, std::unique_ptr<Binding>> bindings;

public:
    Environment() = default;
    Environment(std::shared_ptr<Environment> parent) : parent(std::move(parent)) {}

    std::shared_ptr<Environment> GetParent() const;
    bool HasParent() const;

    void DeclareBinding(std::unique_ptr<Binding> binding);
    std::unique_ptr<Binding> &GetBinding(const std::string &name, BindingKind kind);
    bool BindingExists(const std::string &name, BindingKind kind) const;

    void Print(std::ostream &out) const;
};

#endif
