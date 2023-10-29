#include "ast_node.h"

std::string ASTNode::get_production() {
    std::string result = token.kind;
    for (auto& child : children) {
        result += " " + child.token.kind;
    }
    return result;
}

std::string ASTNode::to_string(int depth) {
    std::string indent = std::string(depth, ' ');
    std::string result;

    result = indent + token.kind + " " + token.lexeme + "\n";
    for (auto& child : children) {
        result += child.to_string(depth + 2);
    }

    return result;
}
