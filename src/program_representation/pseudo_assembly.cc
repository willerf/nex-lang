
#include "pseudo_assembly.h"

#include <vector>

#include "assembly.h"
#include "block.h"
#include "reg.h"
#include "var_access.h"
#include "word.h"

std::shared_ptr<Code> to_expr(Reg reg) {
    return make_add(Reg::Result, reg, Reg::Zero);
}

std::shared_ptr<Code> int_literal(int32_t val) {
    return make_block({make_lis(Reg::Result), make_word((uint32_t)val)});
}

std::shared_ptr<Code>
assign(std::shared_ptr<Variable> var, std::shared_ptr<Code> expr) {
    return make_block({expr, make_write(var, Reg::Result)});
}

std::shared_ptr<Code> assign_to_address(
    std::shared_ptr<Code> addr,
    std::shared_ptr<Code> expr,
    uint32_t offset
) {
    return make_block(
        {addr,
         make_add(Reg::Scratch2, Reg::Result, Reg::Zero),
         expr,
         make_sw(Reg::Result, offset, Reg::Scratch2)}
    );
}

std::shared_ptr<Code> deref(std::shared_ptr<Code> expr, uint32_t offset) {
    return make_block({expr, make_lw(Reg::Result, offset, Reg::Result)});
}
