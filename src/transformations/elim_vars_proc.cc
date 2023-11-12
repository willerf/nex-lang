
#include "elim_vars_proc.h"

#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <vector>

#include "block.h"
#include "var_access.h"
#include "reg.h"

ElimVarsProc::ElimVarsProc(
    std::shared_ptr<Chunk> frame,
    std::shared_ptr<Chunk> param_chunk,
    std::shared_ptr<Variable> param_ptr
) :
    frame {frame},
    param_chunk {param_chunk},
    param_ptr {param_ptr} {}

std::shared_ptr<Code> ElimVarsProc::visit(std::shared_ptr<Code> code) {
    return code;
}

std::shared_ptr<Code> ElimVarsProc::visit(std::shared_ptr<Block> block) {
    std::vector<std::shared_ptr<Code>> result;
    for (auto c : block->code) {
        result.push_back(c->accept(*this));
    }
    return make_block(result);
}

std::shared_ptr<Code> ElimVarsProc::visit(std::shared_ptr<VarAccess> var_access
) {
    if (std::find(
            frame->variables.begin(),
            frame->variables.end(),
            var_access->variable
        )
        != frame->variables.end()) {
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
            std::cerr << "Unsupported access type 2." << std::endl;
            exit(1);
        }
    } else {
        std::shared_ptr<Code> load_param_ptr =
            frame->load(Reg::FramePtr, Reg::Scratch, param_ptr);
        if (var_access->var_access_type == VarAccessType::Read) {
            return make_block(
                {load_param_ptr,
                 param_chunk->load(
                     Reg::Scratch,
                     var_access->reg,
                     var_access->variable
                 )}
            );
        } else if (var_access->var_access_type == VarAccessType::Write) {
            return make_block(
                {load_param_ptr,
                 param_chunk->store(
                     Reg::Scratch,
                     var_access->variable,
                     var_access->reg
                 )}
            );
        } else if (var_access->var_access_type == VarAccessType::Address) {
            return make_block(
                {load_param_ptr,
                 param_chunk->load_address(
                     Reg::Scratch,
                     var_access->reg,
                     var_access->variable
                 )}
            );
        } else {
            std::cerr << "Unsupported access type 3." << std::endl;
            exit(1);
        }
    }
}
