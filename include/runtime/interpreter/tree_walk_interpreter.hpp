#ifndef TREE_WALK_INTERPRETER_HPP_INCLUDED
#define TREE_WALK_INTERPRETER_HPP_INCLUDED

#include "ast/program.hpp"
#include "semantic_analysis/scope.hpp"

class TreeWalkInterpreter
{
private:
    const std::unique_ptr<Program> &program;
    std::shared_ptr<Scope> currentScope;

public:
    TreeWalkInterpreter(const std::unique_ptr<Program> &program)
        : program(program), currentScope(std::make_shared<Scope>()) {}

    void CreateScope();
    void DestroyScope();
    std::shared_ptr<Scope> GetCurrentScope();

    void Run();
};

#endif