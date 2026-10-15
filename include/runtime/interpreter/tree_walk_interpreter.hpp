#ifndef TREE_WALK_INTERPRETER_HPP_INCLUDED
#define TREE_WALK_INTERPRETER_HPP_INCLUDED

#include "ast/program.hpp"
#include "runtime/environment.hpp"

class TreeWalkInterpreter
{
private:
    const std::unique_ptr<Program> &program;
    std::shared_ptr<Environment> currentEnvironment;

public:
    TreeWalkInterpreter(const std::unique_ptr<Program> &program)
        : program(program), currentEnvironment(std::make_shared<Environment>()) {}

    std::shared_ptr<Environment> CreateEnvironment();
    void DestroyEnvironment();
    std::shared_ptr<Environment> GetCurrentEnvironment();

    void Run();
};

#endif
