
#pragma once

#include "token.h"

struct ASTNode {
    Token token;
    std::vector<ASTNode> children;
    std::string to_string(int depth);
};
