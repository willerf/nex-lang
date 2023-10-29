#include "ast_node.h"

std::string ASTNode::to_string(int depth) {
    std::string indent = std::string(depth, ' ');
    std::string result;

    result = indent + token.kind + " " + token.lexeme + "\n";
    for (auto& node : children) {
        result += node.to_string(depth + 2);
    }

    return result;
}
