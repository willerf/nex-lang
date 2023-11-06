
#pragma once

#include <memory>
#include <string>

#include "code.h"

struct Variable: std::enable_shared_from_this<Variable> {
    std::string name;
    explicit Variable(std::string name);
    std::shared_ptr<Code> to_expr(bool read_address = false);
};
