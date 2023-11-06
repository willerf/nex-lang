
#include "variable.h"

#include "reg.h"
#include "var_access.h"

Variable::Variable(std::string name) : name {name} {}

std::shared_ptr<Code> Variable::to_expr(bool read_address) {
    if (read_address) {
        return make_read_address(Reg::Result, shared_from_this());
    } else {
        return make_read(Reg::Result, shared_from_this());
    }
}
