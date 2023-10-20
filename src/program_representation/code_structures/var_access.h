
#pragma once

#include "code_visit.h"
#include "reg.h"
#include "variable.h"

struct VarAccess: CodeVisit<VarAccess> {
    Reg reg;
    std::shared_ptr<Variable> variable;
    bool read;

    VarAccess(Reg reg, std::shared_ptr<Variable> variable, bool read);
};

std::shared_ptr<Code> make_read(Reg reg, std::shared_ptr<Variable> variable);
std::shared_ptr<Code> make_write(std::shared_ptr<Variable> variable, Reg reg);
