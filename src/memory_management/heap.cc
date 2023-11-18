
#include "heap.h"

#include <stdint.h>

#include <vector>

#include "assembly.h"
#include "beq_label.h"
#include "bin_op.h"
#include "block.h"
#include "define_label.h"
#include "if_stmt.h"
#include "label.h"
#include "nl_type_i32.h"
#include "nl_type_none.h"
#include "operators.h"
#include "procedure.h"
#include "pseudo_assembly.h"
#include "reg.h"
#include "scope.h"
#include "typed_variable.h"
#include "var_access.h"
#include "variable.h"

std::shared_ptr<Code> init_heap(std::shared_ptr<Code> heap_start) {
    return make_block(
        {heap_start, make_add(Reg::HeapPtr, Reg::Result, Reg::Zero)}
    );
}

std::shared_ptr<TypedProcedure>
make_heap_allocate(std::shared_ptr<Code> heap_start) {
    std::shared_ptr<Variable> num_bytes =
        std::make_shared<Variable>("num bytes to heap allocate");
    std::shared_ptr<Variable> result =
        std::make_shared<Variable>("pointer to heap block");
    std::shared_ptr<Procedure> proc = std::make_shared<Procedure>(
        "heap allocate",
        std::vector<std::shared_ptr<Variable>> {num_bytes}
    );
    std::shared_ptr<Label> loop_start = std::make_shared<Label>("loop start");
    std::shared_ptr<Label> loop_end = std::make_shared<Label>("loop end");
    proc->code = make_scope(
        {result},
        {heap_start,
         make_add(Reg::HeapPtr, Reg::Result, Reg::Zero),
         make_define(loop_start),
         make_if(
             bin_op(
                 to_expr(Reg::Zero),
                 op::minus(),
                 deref(to_expr(Reg::HeapPtr))
             ),
             op::ge_cmp(),
             num_bytes->to_expr(),
             make_block(
                 {bin_op(
                      to_expr(Reg::Zero),
                      op::minus(),
                      deref(to_expr(Reg::HeapPtr))
                  ),
                  make_sw(Reg::Result, 0, Reg::HeapPtr),
                  make_write(result, Reg::HeapPtr),
                  make_beq(Reg::Zero, Reg::Zero, loop_end)}
             )
         ),
         make_if(
             deref(to_expr(Reg::HeapPtr)),
             op::eq_cmp(),
             to_expr(Reg::Zero),
             make_block(
                 {num_bytes->to_expr(),
                  make_sw(Reg::Result, 0, Reg::HeapPtr),
                  make_write(result, Reg::HeapPtr),
                  make_beq(Reg::Zero, Reg::Zero, loop_end)}
             )
         ),
         make_if(
             deref(to_expr(Reg::HeapPtr)),
             op::lt_cmp(),
             to_expr(Reg::Zero),
             bin_op(
                 to_expr(Reg::Zero),
                 op::minus(),
                 deref(to_expr(Reg::HeapPtr))
             ),
             deref(to_expr(Reg::HeapPtr))
         ),
         make_add(Reg::HeapPtr, Reg::HeapPtr, Reg::Result),
         int_literal(4),
         make_add(Reg::HeapPtr, Reg::HeapPtr, Reg::Result),
         make_beq(Reg::Zero, Reg::Zero, loop_start),
         make_define(loop_end),
         bin_op(result->to_expr(), op::plus(), int_literal(4))}
    );

    std::shared_ptr<TypedVariable> typed_var = std::make_shared<TypedVariable>(
        num_bytes,
        std::make_shared<NLTypeI32>()
    );
    return std::make_shared<TypedProcedure>(
        proc,
        std::make_shared<NLTypeNone>(),
        std::vector<std::shared_ptr<TypedVariable>> {typed_var}
    );
}

std::shared_ptr<TypedProcedure> make_heap_free(std::shared_ptr<Code> heap_start
) {
    std::shared_ptr<Variable> mem_addr =
        std::make_shared<Variable>("address to free");
    std::shared_ptr<Procedure> proc = std::make_shared<Procedure>(
        "heap free",
        std::vector<std::shared_ptr<Variable>> {mem_addr}
    );
    proc->code = make_block({
        mem_addr->to_expr(),
        make_lw(Reg::Scratch, (uint16_t)(-4), Reg::Result),
        make_sub(Reg::Scratch, Reg::Zero, Reg::Scratch),
        make_sw(Reg::Scratch, (uint16_t)(-4), Reg::Result),
    });

    std::shared_ptr<TypedVariable> typed_var = std::make_shared<TypedVariable>(
        mem_addr,
        std::make_shared<NLTypeNone>()
    );
    return std::make_shared<TypedProcedure>(
        proc,
        std::make_shared<NLTypeNone>(),
        std::vector<std::shared_ptr<TypedVariable>> {typed_var}
    );
}
