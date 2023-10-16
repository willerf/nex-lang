
#include "var_access.h"

VarAccess::VarAccess(Reg reg, std::shared_ptr<Variable> variable, bool read) : reg{reg}, variable{variable}, read{read} {}

std::shared_ptr<Code> make_read(Reg reg, std::shared_ptr<Variable> variable) {
    return std::make_shared<VarAccess>(reg, variable, true);
}

std::shared_ptr<Code> make_write(std::shared_ptr<Variable> variable, Reg reg) {
    return std::make_shared<VarAccess>(reg, variable, false);
}

