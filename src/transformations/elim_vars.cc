
#include "elim_vars.h"

#include "block.h"
#include "var_access.h"

ElimVars::ElimVars(Chunk frame) : frame {frame} {}

std::shared_ptr<Code> ElimVars::visit(std::shared_ptr<Code> code) {
    return code;
}

std::shared_ptr<Code> ElimVars::visit(std::shared_ptr<Block> block) {
    std::vector<std::shared_ptr<Code>> result;
    for (auto c : block->code) {
        result.push_back(c->accept(*this));
    }
    return make_block(result);
}

std::shared_ptr<Code> ElimVars::visit(std::shared_ptr<VarAccess> var_access) {
    if (var_access->read) {
        return frame.load(Reg::FramePtr, var_access->reg, var_access->variable);
    } else {
        return frame
            .store(Reg::FramePtr, var_access->variable, var_access->reg);
    }
}
