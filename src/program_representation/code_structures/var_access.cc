
#include "var_access.h"

VarAccess::VarAccess(
    Reg reg,
    std::shared_ptr<Variable> variable,
    VarAccessType var_access_type
) :
    reg {reg},
    variable {variable},
    var_access_type {var_access_type} {}

std::shared_ptr<Code> make_read(Reg reg, std::shared_ptr<Variable> variable) {
    return std::make_shared<VarAccess>(reg, variable, VarAccessType::Read);
}

std::shared_ptr<Code> make_write(std::shared_ptr<Variable> variable, Reg reg) {
    return std::make_shared<VarAccess>(reg, variable, VarAccessType::Write);
}

std::shared_ptr<Code>
make_read_address(Reg reg, std::shared_ptr<Variable> variable) {
    return std::make_shared<VarAccess>(reg, variable, VarAccessType::Address);
}
