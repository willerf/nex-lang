
#include "if_stmt.h"
#include "assembly.h"
#include "beq_label.h"
#include "bne_label.h"
#include "block.h"

IfStmt::IfStmt(std::shared_ptr<Label> else_label, std::shared_ptr<Code> e1, std::shared_ptr<Code> comp, std::shared_ptr<Code> e2, std::shared_ptr<Code> thens, std::shared_ptr<Code> elses) : else_label{else_label}, e1{e1}, comp{comp}, e2{e2}, thens{thens}, elses{elses} {}

std::shared_ptr<Code> make_if(std::shared_ptr<Code> e1, std::function<std::shared_ptr<Code>(std::shared_ptr<Label>)> comp, std::shared_ptr<Code> e2, std::shared_ptr<Code> thens, std::shared_ptr<Code> elses) {
    std::shared_ptr<Label> else_label = std::make_shared<Label>("if stmt else label"); 
    return std::make_shared<IfStmt>(else_label, e1, comp(else_label), e2, thens, elses);
}

std::shared_ptr<Code> eq_cmp(std::shared_ptr<Label> label) {
    return make_bne(Reg::Scratch, Reg::Result, label);
}

std::shared_ptr<Code> ne_cmp(std::shared_ptr<Label> label) {
    return make_beq(Reg::Scratch, Reg::Result, label);
}

std::shared_ptr<Code> lt_cmp(std::shared_ptr<Label> label) {
    return make_block({
        make_slt(Reg::Result, Reg::Scratch, Reg::Result),
        make_beq(Reg::Zero, Reg::Result, label)
    });
}

std::shared_ptr<Code> gt_cmp(std::shared_ptr<Label> label) {
    return make_block({
        make_slt(Reg::Result, Reg::Result, Reg::Scratch),
        make_beq(Reg::Zero, Reg::Result, label)
    });
}

std::shared_ptr<Code> le_cmp(std::shared_ptr<Label> label) {
    return make_block({
        make_slt(Reg::Result, Reg::Result, Reg::Scratch),
        make_bne(Reg::Zero, Reg::Result, label)
    });
}

std::shared_ptr<Code> ge_cmp(std::shared_ptr<Label> label) {
    return make_block({
        make_slt(Reg::Result, Reg::Scratch, Reg::Result),
        make_bne(Reg::Zero, Reg::Result, label)
    });
}

std::shared_ptr<Code> gt_unsigned_cmp(std::shared_ptr<Label> label) {
    return make_block({
        make_sltu(Reg::Result, Reg::Result, Reg::Scratch),
        make_beq(Reg::Zero, Reg::Result, label)
    });
}

