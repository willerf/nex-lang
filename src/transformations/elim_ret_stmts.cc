
#include "elim_ret_stmts.h"

#include <vector>

#include "beq_label.h"
#include "block.h"
#include "reg.h"

ElimRetStmts::ElimRetStmts(std::shared_ptr<Label> proc_end) :
    proc_end {proc_end} {}

std::shared_ptr<Code> ElimRetStmts::visit(std::shared_ptr<Code> code) {
    return code;
}

std::shared_ptr<Code> ElimRetStmts::visit(std::shared_ptr<RetStmt> ret_stmt) {
    return make_block(
        {ret_stmt->code->accept(*this), make_beq(Reg::Zero, Reg::Zero, proc_end)
        }
    );
}
