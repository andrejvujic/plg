#include "runtime/environment.hpp"

std::shared_ptr<Environment> Environment::GetParent() const
{
    return parent;
}

bool Environment::HasParent() const
{
    return parent != nullptr;
}

void Environment::DeclareBinding(std::unique_ptr<Binding> binding)
{
    const auto name = binding->GetName();
    bindings[name] = std::move(binding);
}

std::unique_ptr<Binding> &Environment::GetBinding(const std::string &name, BindingKind kind)
{
    auto iterator = bindings.find(name);
    if (iterator != bindings.end() && iterator->second->IsOfKind(kind))
        return iterator->second;

    if (parent)
        return parent->GetBinding(name, kind);

    std::cerr << "[Environment] ERROR: Binding with given name and kind doesn't exist" << std::endl;
    std::exit(EXIT_FAILURE);
}

bool Environment::BindingExists(const std::string &name, BindingKind kind) const
{
    auto iterator = bindings.find(name);
    if (iterator != bindings.end() && iterator->second->IsOfKind(kind))
        return true;

    return parent ? parent->BindingExists(name, kind) : false;
}

void Environment::Print(std::ostream &out) const
{
    for (const auto &[_, binding] : bindings)
        binding->Print(out);
}
