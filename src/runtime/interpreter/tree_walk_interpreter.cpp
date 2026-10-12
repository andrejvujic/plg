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

void TreeWalkInterpreter::CreateScope()
{
    auto newScope = std::make_shared<Scope>(currentScope);
    currentScope = newScope;
}

void TreeWalkInterpreter::DestroyScope()
{
    if (!currentScope->HasParent())
        return;
    currentScope = currentScope->GetParent();
}