
#pragma once

#include "token.h"

struct ASTNode {
    Token token;
    std::vector<ASTNode> children;
};
