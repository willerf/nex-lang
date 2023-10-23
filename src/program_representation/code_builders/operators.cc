
#include "operators.h"

#include "reg.h"
#include "assembly.h"
#include "block.h"

std::shared_ptr<Code> op::plus() {
    return make_add(Reg::Result, Reg::Scratch, Reg::Result);
}

std::shared_ptr<Code> op::minus() {
    return make_sub(Reg::Result, Reg::Scratch, Reg::Result);
}

std::shared_ptr<Code> op::times() {
    return make_block(
        {make_mult(Reg::Scratch, Reg::Result), make_mflo(Reg::Result)}
    );
}

std::shared_ptr<Code> op::divide() {
    return make_block(
        {make_div(Reg::Scratch, Reg::Result), make_mflo(Reg::Result)}
    );
}

std::shared_ptr<Code> op::remainder() {
    return make_block(
        {make_div(Reg::Scratch, Reg::Result), make_mfhi(Reg::Result)}
    );
}

std::shared_ptr<Code> op::divide_unsigned() {
    return make_block(
        {make_divu(Reg::Scratch, Reg::Result), make_mflo(Reg::Result)}
    );
}

std::shared_ptr<Code> op::remainder_unsigned() {
    return make_block(
        {make_divu(Reg::Scratch, Reg::Result), make_mfhi(Reg::Result)}
    );
}
