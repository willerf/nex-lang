
#include "operators.h"

#include "assembly.h"
#include "beq_label.h"
#include "block.h"
#include "bne_label.h"
#include "define_label.h"
#include "label.h"
#include "reg.h"
#include "word.h"

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

std::shared_ptr<Code> op::and_bool() {
    std::shared_ptr<Label> label1 = std::make_shared<Label>("and label1");
    std::shared_ptr<Label> label2 = std::make_shared<Label>("and label2");
    return make_block(
        {make_beq(Reg::Scratch, Reg::Zero, label1),
         make_beq(Reg::Result, Reg::Zero, label1),
         make_lis(Reg::Result),
         make_word(1),
         make_beq(Reg::Zero, Reg::Zero, label2),
         make_define(label1),
         make_lis(Reg::Result),
         make_word(0),
         make_define(label2)}
    );
}

std::shared_ptr<Code> op::or_bool() {
    std::shared_ptr<Label> label1 = std::make_shared<Label>("and label1");
    std::shared_ptr<Label> label2 = std::make_shared<Label>("and label2");
    return make_block(
        {make_bne(Reg::Scratch, Reg::Zero, label1),
         make_bne(Reg::Result, Reg::Zero, label1),
         make_lis(Reg::Result),
         make_word(0),
         make_beq(Reg::Zero, Reg::Zero, label2),
         make_define(label1),
         make_lis(Reg::Result),
         make_word(1),
         make_define(label2)}
    );
}

std::shared_ptr<Code> op::eq_cmp() {
    std::shared_ptr<Label> label1 = std::make_shared<Label>("and label1");
    std::shared_ptr<Label> label2 = std::make_shared<Label>("and label2");
    return make_block(
        {make_bne(Reg::Scratch, Reg::Result, label1),
         make_lis(Reg::Result),
         make_word(1),
         make_beq(Reg::Zero, Reg::Zero, label2),
         make_define(label1),
         make_lis(Reg::Result),
         make_word(0),
         make_define(label2)}
    );
}

std::shared_ptr<Code> op::ne_cmp() {
    std::shared_ptr<Label> label1 = std::make_shared<Label>("and label1");
    std::shared_ptr<Label> label2 = std::make_shared<Label>("and label2");
    return make_block(
        {make_beq(Reg::Scratch, Reg::Result, label1),
         make_lis(Reg::Result),
         make_word(1),
         make_beq(Reg::Zero, Reg::Zero, label2),
         make_define(label1),
         make_lis(Reg::Result),
         make_word(0),
         make_define(label2)}
    );
}

std::shared_ptr<Code> op::lt_cmp() {
    return make_slt(Reg::Result, Reg::Scratch, Reg::Result);
}

std::shared_ptr<Code> op::gt_cmp() {
    return make_slt(Reg::Result, Reg::Result, Reg::Scratch);
}

std::shared_ptr<Code> op::le_cmp() {
    std::shared_ptr<Label> label1 = std::make_shared<Label>("ge label1");
    std::shared_ptr<Label> label2 = std::make_shared<Label>("ge label2");
    return make_block(
        {make_slt(Reg::Result, Reg::Result, Reg::Scratch),
         make_beq(Reg::Result, Reg::Zero, label1),
         make_lis(Reg::Result),
         make_word(0),
         make_beq(Reg::Zero, Reg::Zero, label2),
         make_define(label1),
         make_lis(Reg::Result),
         make_word(1),
         make_define(label2)}
    );
}

std::shared_ptr<Code> op::ge_cmp() {
    std::shared_ptr<Label> label1 = std::make_shared<Label>("ge label1");
    std::shared_ptr<Label> label2 = std::make_shared<Label>("ge label2");
    return make_block(
        {make_slt(Reg::Result, Reg::Scratch, Reg::Result),
         make_beq(Reg::Result, Reg::Zero, label1),
         make_lis(Reg::Result),
         make_word(0),
         make_beq(Reg::Zero, Reg::Zero, label2),
         make_define(label1),
         make_lis(Reg::Result),
         make_word(1),
         make_define(label2)}
    );
}
