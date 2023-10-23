
#include "comparators.h"

#include "assembly.h"
#include "beq_label.h"
#include "block.h"
#include "bne_label.h"
#include "reg.h"

std::shared_ptr<Code> eq_cmp(std::shared_ptr<Label> label) {
    return make_bne(Reg::Scratch, Reg::Result, label);
}

std::shared_ptr<Code> ne_cmp(std::shared_ptr<Label> label) {
    return make_beq(Reg::Scratch, Reg::Result, label);
}

std::shared_ptr<Code> lt_cmp(std::shared_ptr<Label> label) {
    return make_block(
        {make_slt(Reg::Result, Reg::Scratch, Reg::Result),
         make_beq(Reg::Zero, Reg::Result, label)}
    );
}

std::shared_ptr<Code> gt_cmp(std::shared_ptr<Label> label) {
    return make_block(
        {make_slt(Reg::Result, Reg::Result, Reg::Scratch),
         make_beq(Reg::Zero, Reg::Result, label)}
    );
}

std::shared_ptr<Code> le_cmp(std::shared_ptr<Label> label) {
    return make_block(
        {make_slt(Reg::Result, Reg::Result, Reg::Scratch),
         make_bne(Reg::Zero, Reg::Result, label)}
    );
}

std::shared_ptr<Code> ge_cmp(std::shared_ptr<Label> label) {
    return make_block(
        {make_slt(Reg::Result, Reg::Scratch, Reg::Result),
         make_bne(Reg::Zero, Reg::Result, label)}
    );
}

std::shared_ptr<Code> gt_unsigned_cmp(std::shared_ptr<Label> label) {
    return make_block(
        {make_sltu(Reg::Result, Reg::Result, Reg::Scratch),
         make_beq(Reg::Zero, Reg::Result, label)}
    );
}
