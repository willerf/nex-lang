
#include <stdint.h>

#include <catch2/catch_test_macros.hpp>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

#include "assembly.h"
#include "bin_op.h"
#include "block.h"
#include "chunk.h"
#include "elim_if_stmts.h"
#include "elim_labels.h"
#include "elim_scopes.h"
#include "elim_vars.h"
#include "flatten.h"
#include "if_stmt.h"
#include "operators.h"
#include "pseudo_assembly.h"
#include "reg.h"
#include "scope.h"
#include "stack.h"
#include "utils.h"
#include "variable.h"
#include "while_loop.h"
#include "word.h"
#include "write_file.h"

struct Code;

static uint32_t TERMINATION_PC = 0b11111110111000011101111010101101;
static std::string file_name("test_fibonacci.bin");

// representation of test program
int32_t sample_fibonacci(int32_t n) {
    int32_t result = 0;
    int32_t v1 = 0;
    int32_t v2 = 1;
    if (n == 0) {
        result = v1;
    }

    if (n == 1) {
        result = v2;
    }

    if (n >= 2) {
        int32_t i = 1;
        int32_t v3 = 0;
        while (i < n) {
            v3 = v1 + v2;
            v1 = v2;
            v2 = v3;
            i++;
        }
        result = v3;
    }

    return result;
}

TEST_CASE("fibonacci program", "[programs]") {
    auto v1 = std::make_shared<Variable>("v1");
    auto v2 = std::make_shared<Variable>("v2");
    auto v3 = std::make_shared<Variable>("v3");
    auto i = std::make_shared<Variable>("i");
    auto result = std::make_shared<Variable>("result");

    auto program = make_scope(
        {v1, v2, result},
        {assign(result, int_literal(0)),
         assign(v1, int_literal(0)),
         assign(v2, int_literal(1)),
         make_if(
             to_expr(Reg::Input1),
             op::eq_cmp(),
             int_literal(0),
             assign(result, v1->to_expr())
         ),
         make_if(
             to_expr(Reg::Input1),
             op::eq_cmp(),
             int_literal(1),
             assign(result, v2->to_expr())
         ),
         make_if(
             to_expr(Reg::Input1),
             op::ge_cmp(),
             int_literal(2),
             make_scope(
                 {i, v3},
                 {assign(i, int_literal(1)),
                  make_while(
                      i->to_expr(),
                      op::lt_cmp(),
                      to_expr(Reg::Input1),
                      make_block(
                          {assign(
                               v3,
                               bin_op(v1->to_expr(), op::plus(), v2->to_expr())
                           ),
                           assign(v1, v2->to_expr()),
                           assign(v2, v3->to_expr()),
                           assign(
                               i,
                               bin_op(i->to_expr(), op::plus(), int_literal(1))
                           )}
                      )
                  ),
                  assign(result, v3->to_expr())}
             )
         ),
         result->to_expr()}
    );

    ElimIfStmts elim_if_stmts;
    auto program2 = program->accept(elim_if_stmts);

    ElimScopes elim_scopes;
    auto program3 = program2->accept(elim_scopes);
    auto chunk = std::make_shared<Chunk>(elim_scopes.get());
    ElimVars elim_vars(chunk);

    std::shared_ptr<Code> program4 = program3->accept(elim_vars);

    auto program5 = make_block(
        {stack::allocate(chunk),
         make_add(Reg::FramePtr, Reg::StackPtr, Reg::Zero),
         program4,
         stack::pop()}
    );

    Flatten flatten;
    program5->accept(flatten);

    auto program6 = flatten.get();

    program6.push_back(make_lis(Reg::TargetPC));
    program6.push_back(make_word(TERMINATION_PC));
    program6.push_back(make_jr(Reg::TargetPC));

    auto program7 = elim_labels(program6);

    write_file(file_name, program7);

    for (auto input : {0, 1, 2, 3, 5, 10}) {
        REQUIRE(stoi(emulate(file_name, input, 0)) == sample_fibonacci(input));
    }
}
