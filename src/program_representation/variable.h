
#pragma once

#include <string>

struct Variable {
    std::string name;
    bool is_pointer;
    explicit Variable(std::string name, bool is_pointer = false);
};
