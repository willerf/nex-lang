
#pragma once

#include "code_visit.h"
#include "reg.h"
#include "variable.h"

enum class VarAccessType { Read, Write, Address };

struct VarAccess: CodeVisit<VarAccess> {
    Reg reg;
    std::shared_ptr<Variable> variable;
    VarAccessType var_access_type;

    VarAccess(
        Reg reg,
        std::shared_ptr<Variable> variable,
        VarAccessType var_access_type
    );
};

std::shared_ptr<Code> make_read(Reg reg, std::shared_ptr<Variable> variable);
std::shared_ptr<Code>
make_read_address(Reg reg, std::shared_ptr<Variable> variable);
std::shared_ptr<Code> make_write(std::shared_ptr<Variable> variable, Reg reg);
