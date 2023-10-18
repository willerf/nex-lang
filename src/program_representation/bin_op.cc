
#include "bin_op.h"

#include "variable.h"
#include "block.h"
#include "var_access.h"
#include "scope.h"

std::shared_ptr<Code> bin_op(std::shared_ptr<Code> e1, std::shared_ptr<Code> op, std::shared_ptr<Code> e2) {
    std::shared_ptr<Variable> var = std::make_shared<Variable>("bin_op variable");
    return make_scope({var},
        make_block({
            e1,
            make_write(var, Reg::Result),
            e2,
            make_read(Reg::Scratch, var),
            op
        })
    );
}

std::shared_ptr<Code> op::plus() {
    return make_add(Reg::Result, Reg::Scratch, Reg::Result);
}

std::shared_ptr<Code> op::minus() {
    return make_sub(Reg::Result, Reg::Scratch, Reg::Result);
}

std::shared_ptr<Code> op::times() {
    return make_block({
        make_mult(Reg::Scratch, Reg::Result),
        make_mflo(Reg::Result)
    });
}

std::shared_ptr<Code> op::divide() {
    return make_block({
        make_div(Reg::Scratch, Reg::Result),
        make_mflo(Reg::Result)
    });
}

std::shared_ptr<Code> op::remainder() {
    return make_block({
        make_div(Reg::Scratch, Reg::Result),
        make_mfhi(Reg::Result)
    });
}

std::shared_ptr<Code> op::divide_unsigned() {
    return make_block({
        make_divu(Reg::Scratch, Reg::Result),
        make_mflo(Reg::Result)
    });
}

std::shared_ptr<Code> op::remainder_unsigned() {
    return make_block({
        make_divu(Reg::Scratch, Reg::Result),
        make_mfhi(Reg::Result)
    });
}

