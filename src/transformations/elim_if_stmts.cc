
#include "elim_if_stmts.h"

#include "../program_representation/bin_op.h"
#include "../program_representation/code_structures/if_stmt.h"
#include "../program_representation/code_structures/beq_label.h"
#include "../program_representation/code_structures/define_label.h"

std::shared_ptr<Code> ElimIfStmts::visit(std::shared_ptr<Code> code) {
    return code;
}



std::shared_ptr<Code> ElimIfStmts::visit(std::shared_ptr<IfStmt> if_stmt) {
    std::shared_ptr<Label> end_label = std::make_shared<Label>("ifstmt end label");
    return make_block({
        bin_op(if_stmt->e1->accept(*this), if_stmt->comp, if_stmt->e2),
        if_stmt->thens,
        make_beq(Reg::Zero, Reg::Zero, end_label),
        make_define(if_stmt->else_label),
        if_stmt->elses,
        make_define(end_label)
    });
}

