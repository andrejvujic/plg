#ifndef SCOPED_NEW_SCOPE_HPP_INCLUDED
#define SCOPED_NEW_SCOPE_HPP_INCLUDED

#include "runtime/interpreter/tree_walk_interpreter.hpp"

class ScopedNewScope
{
private:
    TreeWalkInterpreter &interpreter;

public:
    explicit ScopedNewScope(TreeWalkInterpreter &interpreter) : interpreter(interpreter)
    {
        interpreter.CreateScope();
    }

    ScopedNewScope(const ScopedNewScope &) = delete;
    ScopedNewScope &operator=(const ScopedNewScope &) = delete;

    ~ScopedNewScope()
    {
        interpreter.DestroyScope();
    }
};

#endif
