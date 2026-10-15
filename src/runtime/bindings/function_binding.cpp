#include "runtime/bindings/function_binding.hpp"

const FunctionDeclaration &FunctionBinding::GetDeclaration() const
{
    return *declaration;
}

void FunctionBinding::Print(std::ostream &out) const
{
    out << GetType() << " " << GetName() << "(";
    const auto &parameters = declaration->GetParameters();
    for (size_t index = 0; index < parameters.size(); ++index)
    {
        out << parameters[index]->GetVarType();
        if (index < parameters.size() - 1)
            out << ", ";
    }
    out << ")" << std::endl;
}
