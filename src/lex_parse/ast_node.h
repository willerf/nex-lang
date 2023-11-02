
#pragma once

#include "state.h"
#include <string>

struct ASTNode {
    State state;
    std::string lexeme;
    std::vector<ASTNode> children;
    std::vector<State> get_production();
    std::string to_string(int depth);
};
