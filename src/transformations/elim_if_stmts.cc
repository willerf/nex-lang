
#include "elim_if_stmts.h"

#include "beq_label.h"
#include "bin_op.h"
#include "define_label.h"
#include "if_stmt.h"

std::shared_ptr<Code> ElimIfStmts::visit(std::shared_ptr<Code> code) {
    return code;
}

std::shared_ptr<Code> ElimIfStmts::visit(std::shared_ptr<IfStmt> if_stmt) {
    std::shared_ptr<Label> end_label =
        std::make_shared<Label>("if stmt end label");
    return make_block(
        {bin_op(if_stmt->e1->accept(*this), if_stmt->comp->accept(*this), if_stmt->e2->accept(*this)),
         if_stmt->thens->accept(*this),
         make_beq(Reg::Zero, Reg::Zero, end_label),
         make_define(if_stmt->else_label),
         if_stmt->elses->accept(*this),
         make_define(end_label)}
    );
}
