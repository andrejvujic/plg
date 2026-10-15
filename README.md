# PLG

PLG is a small statically typed programming language implemented in C++17. The
project contains a lexer, parser, semantic analyzer, and tree-walk interpreter.

## Features

- `int`, `uint`, and `bool` values
- Variable declarations and assignments
- Arithmetic, comparison, and logical expressions
- `if` statements and `for` loops
- Function declarations, calls, parameters, and return values
- Recursive and nested function calls
- Lexical environments and semantic symbol tables

## Example

```plg
function pow(int value, int exponent) -> int {
    int result = 1;
    int i;

    for (i = 0; i < exponent; i = i + 1)
        result = result * value;

    return result;
}

int answer = pow(2, 5);
```

## Generated AST

The parser represents the example above with the following AST, as printed by
`AstPrinter`:

```text
[Program] {
  [FunctionDeclaration] {
    Name -> pow
    Return Type -> int
    Body -> [ComplexStatement] {
      [VariableDeclaration] {
        Var. Name -> result
        Var. Type -> int
        Initial Value -> [NumLiteralExpression] {
          Raw Value -> 1
          Literal Type -> int
        }
      }
      [VariableDeclaration] {
        Var. Name -> i
        Var. Type -> int
        Initial Value -> null
      }
      [ForStatement] {
        Init -> [AssignmentExpression] {
          Assignee -> [IdExpression] {
            Value -> i
          }
          Value -> [NumLiteralExpression] {
            Raw Value -> 0
            Literal Type -> int
          }
        }
        Condition -> [BinaryExpression] {
          Operator -> <
          Left -> [IdExpression] {
            Value -> i
          }
          Right -> [IdExpression] {
            Value -> exponent
          }
        }
        Update -> [AssignmentExpression] {
          Assignee -> [IdExpression] {
            Value -> i
          }
          Value -> [BinaryExpression] {
            Operator -> +
            Left -> [IdExpression] {
              Value -> i
            }
            Right -> [NumLiteralExpression] {
              Raw Value -> 1
              Literal Type -> int
            }
          }
        }
        Body -> [ExpressionStatement] {
          Expression -> [AssignmentExpression] {
            Assignee -> [IdExpression] {
              Value -> result
            }
            Value -> [BinaryExpression] {
              Operator -> *
              Left -> [IdExpression] {
                Value -> result
              }
              Right -> [IdExpression] {
                Value -> value
              }
            }
          }
        }
      }
      [ReturnStatement] {
        Value -> [IdExpression] {
          Value -> result
        }
      }
    }
    Parameter Count -> 2
    [Parameters] {
      [FunctionParameter] {
        Var. Type -> int
        Var. Name -> value
      }
      [FunctionParameter] {
        Var. Type -> int
        Var. Name -> exponent
      }
    }
  }
  [VariableDeclaration] {
    Var. Name -> answer
    Var. Type -> int
    Initial Value -> [CallExpression] {
      Callee -> [IdExpression] {
        Value -> pow
      }
      Argument Count -> 2
      [Arguments] {
        [NumLiteralExpression] {
          Raw Value -> 2
          Literal Type -> int
        }
        [NumLiteralExpression] {
          Raw Value -> 5
          Literal Type -> int
        }
      }
    }
  }
}
```

More programs are available in the [`samples`](samples) directory.

## Building and running

You need a C++17 compiler and Make.

```sh
make
./bin/main
```

You can also build and run the interpreter with one command:

```sh
make run
```

The input file is currently selected by the `path` variable in
[`src/main.cpp`](src/main.cpp). Change it to run a different sample.

To remove generated files:

```sh
make clean
```

## Project structure

```text
include/                  Header files
src/                      Implementations
samples/                  Example PLG programs

include/ast/              Abstract syntax tree nodes
include/lexer/            Tokenization
include/parser/           Parsing
include/semantic_analysis Symbol table and semantic checks
include/runtime/          Interpreter, environments, bindings, and values
```

Semantic analysis uses a `SymbolTable` containing information about declared
variables and functions. During execution, the interpreter uses an
`Environment` containing their runtime bindings and values.
