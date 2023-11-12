
#pragma once

#include <stddef.h>

#include <string>
#include <vector>

#include "state.h"

struct ASTNode {
    State state;
    std::string lexeme;
    std::vector<ASTNode> children;
    size_t line_no = 0;
    std::vector<State> get_production();
    std::string to_string(int depth);
};
