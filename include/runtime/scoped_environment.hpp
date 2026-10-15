#ifndef SCOPED_ENVIRONMENT_HPP_INCLUDED
#define SCOPED_ENVIRONMENT_HPP_INCLUDED

#include "runtime/interpreter/tree_walk_interpreter.hpp"

class ScopedEnvironment
{
private:
    TreeWalkInterpreter &interpreter;

public:
    explicit ScopedEnvironment(TreeWalkInterpreter &interpreter) : interpreter(interpreter)
    {
        interpreter.CreateEnvironment();
    }

    ScopedEnvironment(const ScopedEnvironment &) = delete;
    ScopedEnvironment &operator=(const ScopedEnvironment &) = delete;

    ~ScopedEnvironment()
    {
        interpreter.DestroyEnvironment();
    }
};

#endif
