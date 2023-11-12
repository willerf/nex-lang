
#include <catch2/catch_test_macros.hpp>
#include <stdint.h>
#include <map>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

#include "assembly.h"
#include "bin_op.h"
#include "block.h"
#include "chunk.h"
#include "elim_calls.h"
#include "elim_if_stmts.h"
#include "elim_labels.h"
#include "elim_scopes.h"
#include "elim_vars_proc.h"
#include "entry_exit.h"
#include "flatten.h"
#include "if_stmt.h"
#include "operators.h"
#include "procedure.h"
#include "pseudo_assembly.h"
#include "reg.h"
#include "utils.h"
#include "variable.h"
#include "word.h"
#include "write_file.h"
#include "call.h"
#include "scope.h"

struct Code;

static uint32_t TERMINATION_PC = 0b11111110111000011101111010101101;
static std::string file_name("test_factorial.bin");

// representation of test program
uint32_t sample_factorial(uint32_t n) {
    uint32_t result = 0;
    if (n == 0) {
        result = 1;
    } else {
        result = n * sample_factorial(n - 1);
    }
    return result;
}

uint32_t sample_main(uint32_t input1, uint32_t input2) {
    (void)input2;
    return sample_factorial(input1);
}

TEST_CASE("Test factorial program", "[programs]") {
    auto n = std::make_shared<Variable>("n");
    auto input1 = std::make_shared<Variable>("input1");
    auto input2 = std::make_shared<Variable>("input2");

    auto factorial_proc = std::make_shared<Procedure>(
        "factorial_proc",
        std::vector<std::shared_ptr<Variable>> {n}
    );
    auto main_proc = std::make_shared<Procedure>(
        "main_proc",
        std::vector<std::shared_ptr<Variable>> {input1, input2}
    );

    auto result = std::make_shared<Variable>("result");
    factorial_proc->code = make_scope(
        {result},
        {assign(result, int_literal(0)),
         make_if(
             n->to_expr(),
             op::eq_cmp(),
             int_literal(0),
             assign(result, int_literal(1)),
             assign(
                 result,
                 bin_op(
                     n->to_expr(),
                     op::times(),
                     make_call(
                         factorial_proc,
                         {bin_op(n->to_expr(), op::minus(), int_literal(1))}
                     )
                 )
             )
         ),
         result->to_expr()}
    );

    main_proc->code =
        make_block({make_call(factorial_proc, {input1->to_expr()})});

    std::map<std::shared_ptr<Procedure>, std::shared_ptr<Chunk>> param_chunks;
    for (auto proc : {main_proc, factorial_proc}) {
        param_chunks[proc] = std::make_shared<Chunk>(proc->parameters);
    }

    std::vector<std::shared_ptr<Procedure>> procedures = {
        main_proc,
        factorial_proc};

    auto start_proc = std::make_shared<Procedure>(
        "start_proc",
        std::vector<std::shared_ptr<Variable>> {}
    );
    start_proc->code = make_block(
        {make_call(main_proc, {to_expr(Reg::Input1), to_expr(Reg::Input2)}),
         make_lis(Reg::TargetPC),
         make_word(TERMINATION_PC),
         make_jr(Reg::TargetPC)}
    );
    procedures.insert(procedures.begin(), start_proc);

    for (auto proc : procedures) {
        ElimCalls elim_calls {proc, param_chunks};
        proc->code = proc->code->accept(elim_calls);

        ElimIfStmts elim_if_stmts;
        proc->code = proc->code->accept(elim_if_stmts);

        ElimScopes elim_scopes;
        proc->code = proc->code->accept(elim_scopes);
        auto local_vars = elim_scopes.get();

        std::vector<std::shared_ptr<Variable>> all_local_vars = {
            proc->param_ptr,
            proc->dynamic_link,
            proc->saved_pc};
        all_local_vars
            .insert(all_local_vars.end(), local_vars.begin(), local_vars.end());
        std::shared_ptr<Chunk> local_vars_chunk =
            std::make_shared<Chunk>(all_local_vars);

        proc->code = add_entry_exit(proc, local_vars_chunk);

        auto param_chunk = std::make_shared<Chunk>(proc->parameters);
        ElimVarsProc elim_vars_proc(
            local_vars_chunk,
            param_chunk,
            proc->param_ptr
        );
        proc->code = proc->code->accept(elim_vars_proc);
    }

    std::vector<std::shared_ptr<Code>> all_code;
    for (auto proc : procedures) {
        all_code.push_back(proc->code);
    }
    auto program = make_block(all_code);

    Flatten flatten;
    program->accept(flatten);

    auto program2 = flatten.get();

    auto program3 = elim_labels(program2);

    write_file(file_name, program3);

    for (auto input : {0, 1, 2, 3, 5, 6, 7, 8, 9}) {
        REQUIRE(stoi(emulate(file_name, input, 0)) == sample_main(input, 0));
    }
}
