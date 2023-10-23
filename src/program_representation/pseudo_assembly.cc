
#include "pseudo_assembly.h"
#include "block.h"
#include "var_access.h"
#include "assembly.h"
#include "word.h"

std::shared_ptr<Code> to_expr(Reg reg) {
    return make_add(Reg::Result, reg, Reg::Zero);
}

std::shared_ptr<Code> int_literal(int32_t val) {
    return make_block({
        make_lis(Reg::Result),
        make_word((uint32_t)val)
    });
}

std::shared_ptr<Code> assign(std::shared_ptr<Variable> var, std::shared_ptr<Code> expr) {
    return make_block({
        expr,
        make_write(var, Reg::Result)
    });
}
