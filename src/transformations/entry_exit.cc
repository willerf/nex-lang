
#include "entry_exit.h"

#include "variable.h"
#include "assembly.h"
#include "define_label.h"
#include "reg.h"
#include "stack.h"
#include "block.h"

std::shared_ptr<Code> add_entry_exit(std::shared_ptr<Procedure> proc, std::shared_ptr<Chunk> frame) { 
        std::shared_ptr<Code> proc_start = make_block({
        make_add(Reg::SavedParamPtr, Reg::Result, Reg::Zero),
        stack::allocate(frame),
        frame->store(Reg::Result, proc->dynamic_link, Reg::FramePtr),
        make_add(Reg::FramePtr, Reg::Result, Reg::Zero),
        frame->store(Reg::FramePtr, proc->saved_pc, Reg::Link),
        frame->store(Reg::FramePtr, proc->param_ptr, Reg::SavedParamPtr)
    });

    std::shared_ptr<Code> proc_end = make_block({
        frame->load(Reg::FramePtr, Reg::Link, proc->saved_pc),
        frame->load(Reg::FramePtr, Reg::FramePtr, proc->dynamic_link),
        stack::pop(),
        stack::pop(),
        make_jr(Reg::Link)
    });

    return make_block({
        make_define(proc->label),
        proc_start,
        proc->code,
        proc_end
    });
}

