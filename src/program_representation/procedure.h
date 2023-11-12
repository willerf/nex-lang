
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "code.h"
#include "label.h"
#include "variable.h"

struct Code;
struct Label;
struct Variable;

struct Procedure {
    std::string name;
    std::vector<std::shared_ptr<Variable>> parameters;
    std::shared_ptr<Variable> param_ptr;
    std::shared_ptr<Variable> dynamic_link;
    std::shared_ptr<Variable> saved_pc;
    std::shared_ptr<Label> start_label;
    std::shared_ptr<Label> end_label;
    std::shared_ptr<Code> code;
    Procedure(
        std::string name,
        std::vector<std::shared_ptr<Variable>> parameters
    );
};
