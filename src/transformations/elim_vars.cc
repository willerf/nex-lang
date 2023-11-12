
#include "elim_vars.h"

#include <stdlib.h>

#include <iostream>
#include <vector>

#include "block.h"
#include "reg.h"
#include "var_access.h"

ElimVars::ElimVars(std::shared_ptr<Chunk> frame) : frame {frame} {}

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
    if (var_access->var_access_type == VarAccessType::Read) {
        return frame
            ->load(Reg::FramePtr, var_access->reg, var_access->variable);
    } else if (var_access->var_access_type == VarAccessType::Write) {
        return frame
            ->store(Reg::FramePtr, var_access->variable, var_access->reg);
    } else if (var_access->var_access_type == VarAccessType::Address) {
        return frame->load_address(
            Reg::FramePtr,
            var_access->reg,
            var_access->variable
        );
    } else {
        std::cerr << "Unsupported access type 1." << std::endl;
        exit(1);
    }
}
