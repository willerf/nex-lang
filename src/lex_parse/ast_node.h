
#pragma once

#include <string>

#include "state.h"

struct ASTNode {
    State state;
    std::string lexeme;
    std::vector<ASTNode> children;
    std::vector<State> get_production();
    std::string to_string(int depth);
};
