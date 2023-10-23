
#pragma once

#include <string>
#include <memory>

#include "code.h"

struct Variable : std::enable_shared_from_this<Variable> {
    std::string name;
    bool is_pointer;
    explicit Variable(std::string name, bool is_pointer = false);
    std::shared_ptr<Code> to_expr();
};
