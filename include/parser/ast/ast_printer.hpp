#ifndef AST_PRINTER_HPP_INCLUDED
#define AST_PRINTER_HPP_INCLUDED

#include <cstddef>
#include <ostream>
#include <string_view>

class AstNode;

class AstPrinter
{
private:
    std::ostream &out;
    size_t depth = 0;
    bool isAtLineStart = true, skipNull = false;

public:
    AstPrinter(std::ostream &out, bool skipNull = false);

    void BeginNode(std::string_view name);
    void Field(std::string_view name, std::string_view value);
    void Field(std::string_view name, const AstNode *node);
    void EndNode();
};

#endif
