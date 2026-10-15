#include <map>

#include "ast/ast_node_type.hpp"

std::ostream &operator<<(std::ostream &out, const AstNodeType &type)
{
    static const std::map<AstNodeType, std::string_view> types = []()
    {
        std::map<AstNodeType, std::string_view> types;
#define INSERT_ELEMENT(p) types.emplace(p, #p)
        INSERT_ELEMENT(AstNodeType::AssignmentExpression);
        INSERT_ELEMENT(AstNodeType::IdExpression);
        INSERT_ELEMENT(AstNodeType::CallExpression);
        INSERT_ELEMENT(AstNodeType::BinaryExpression);
        INSERT_ELEMENT(AstNodeType::NumLiteralExpression);
        INSERT_ELEMENT(AstNodeType::BoolLiteralExpression);
        INSERT_ELEMENT(AstNodeType::ComplexStatement);
        INSERT_ELEMENT(AstNodeType::ExpressionStatement);
        INSERT_ELEMENT(AstNodeType::ForStatement);
        INSERT_ELEMENT(AstNodeType::FunctionDeclaration);
        INSERT_ELEMENT(AstNodeType::FunctionParameter);
        INSERT_ELEMENT(AstNodeType::IfStatement);
        INSERT_ELEMENT(AstNodeType::VariableDeclaration);
        INSERT_ELEMENT(AstNodeType::ReturnStatement);
        INSERT_ELEMENT(AstNodeType::Program);
#undef INSERT_ELEMENT
        return types;
    }();

    for (auto iterator = types.begin(); iterator != types.end(); ++iterator)
    {
        if (iterator->first == type)
        {
            out << iterator->second;
            return out;
        }
    }

    out << "AstNodeType::Unknown";
    return out;
}