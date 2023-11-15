
#include "heap.h"
#include "assembly.h"
#include "block.h"
#include "if_stmt.h"
#include "nl_type_i32.h"
#include "nl_type_none.h"
#include "operators.h"
#include "pseudo_assembly.h"
#include "reg.h"
#include "scope.h"
#include "bin_op.h"

std::shared_ptr<Code> init_heap(std::shared_ptr<Code> heap_start) {
    return make_block({
        heap_start,
        make_add(Reg::HeapPtr, Reg::Result, Reg::Zero)
    });
}

std::shared_ptr<TypedProcedure> make_heap_allocate(std::shared_ptr<Code> heap_start, std::shared_ptr<Code> heap_end) {

    std::shared_ptr<Variable> num_bytes = std::make_shared<Variable>("num bytes to heap allocate");
    std::shared_ptr<Variable> result = std::make_shared<Variable>("pointer to heap block");
    std::shared_ptr<Procedure> proc = std::make_shared<Procedure>("heap allocate", std::vector<std::shared_ptr<Variable>>{num_bytes});
    proc->code = make_scope({result}, {
        make_if(
            bin_op(to_expr(Reg::HeapPtr), op::plus(), num_bytes->to_expr()),
            op::ge_cmp(),
            heap_end,
            make_div(Reg::Zero, Reg::Zero)
        ),
        assign_to_address(to_expr(Reg::HeapPtr), num_bytes->to_expr()),
        int_literal(4),
        make_add(Reg::HeapPtr, Reg::HeapPtr, Reg::Result),
        assign(result, to_expr(Reg::HeapPtr)),
        num_bytes->to_expr(),
        make_add(Reg::HeapPtr, Reg::HeapPtr, Reg::Result),
        result->to_expr()
    });

    std::shared_ptr<TypedVariable> typed_var = std::make_shared<TypedVariable>(num_bytes, std::make_shared<NLTypeI32>());
    return std::make_shared<TypedProcedure>(proc, std::make_shared<NLTypeNone>(), std::vector<std::shared_ptr<TypedVariable>>{typed_var});
}

std::shared_ptr<TypedProcedure> make_heap_free(std::shared_ptr<Code> heap_start) {

    std::shared_ptr<Variable> mem_addr = std::make_shared<Variable>("address to free");
    std::shared_ptr<Procedure> proc = std::make_shared<Procedure>("heap free", std::vector<std::shared_ptr<Variable>>{mem_addr});
    proc->code = make_block({
        mem_addr->to_expr(),
        make_lw(Reg::Scratch, (uint16_t)(-4), Reg::Result),
        make_sub(Reg::Scratch, Reg::Zero, Reg::Scratch),
        make_sw(Reg::Scratch, (uint16_t)(-4), Reg::Result),
    });

    std::shared_ptr<TypedVariable> typed_var = std::make_shared<TypedVariable>(mem_addr, std::make_shared<NLTypeNone>());
    return std::make_shared<TypedProcedure>(proc, std::make_shared<NLTypeNone>(), std::vector<std::shared_ptr<TypedVariable>>{typed_var});   
}
