#include "runtime/interpreter/tree_walk_interpreter.hpp"
#include <iostream>

void TreeWalkInterpreter::Run()
{
    program->Execute(*this);
    currentEnvironment->Print(std::cout);
}

std::shared_ptr<Environment> TreeWalkInterpreter::GetCurrentEnvironment()
{
    return currentEnvironment;
}

std::shared_ptr<Environment> TreeWalkInterpreter::CreateEnvironment()
{
    auto environment = std::make_shared<Environment>(currentEnvironment);
    currentEnvironment = environment;
    return currentEnvironment;
}

void TreeWalkInterpreter::DestroyEnvironment()
{
    if (!currentEnvironment->HasParent())
        return;
    currentEnvironment = currentEnvironment->GetParent();
}
