
#pragma once

#include <vector>
#include <memory>

#include "variable.h"
#include "label.h"
#include "code.h"

struct Procedure { 
    std::string name;
    std::vector<std::shared_ptr<Variable>> parameters;
    std::shared_ptr<Variable> param_ptr;
    std::shared_ptr<Variable> dynamic_link;
    std::shared_ptr<Variable> saved_pc;
    std::shared_ptr<Label> label;
    std::shared_ptr<Code> code;
    Procedure(std::string name, std::vector<std::shared_ptr<Variable>> parameters);
};

