#include "runtime/interpreter/tree_walk_interpreter.hpp"
#include <iostream>

void TreeWalkInterpreter::Run()
{
    program->Execute(*this);
    currentScope->Print(std::cout);
}

std::shared_ptr<Scope> TreeWalkInterpreter::GetCurrentScope()
{
    return currentScope;
}

std::shared_ptr<Scope> TreeWalkInterpreter::CreateScope()
{
    auto newScope = std::make_shared<Scope>(currentScope);
    currentScope = newScope;
    return currentScope;
}

void TreeWalkInterpreter::DestroyScope()
{
    if (!currentScope->HasParent())
        return;
    currentScope = currentScope->GetParent();
}