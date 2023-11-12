
#pragma once

#include <memory>
#include <vector>

#include "code.h"
#include "code_visit.h"
#include "reg.h"
#include "variable.h"

struct Variable;

struct Scope: CodeVisit<Scope> {
    std::vector<std::shared_ptr<Variable>> variables;
    std::shared_ptr<Code> code;

    Scope(
        std::vector<std::shared_ptr<Variable>> variables,
        std::shared_ptr<Code> code
    );
};

std::shared_ptr<Code> make_scope(
    std::vector<std::shared_ptr<Variable>> variables,
    std::shared_ptr<Code> code
);

std::shared_ptr<Code> make_scope(
    std::vector<std::shared_ptr<Variable>> variables,
    std::vector<std::shared_ptr<Code>> code
);
