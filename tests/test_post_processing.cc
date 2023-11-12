

#include <catch2/catch_test_macros.hpp>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include "assembly.h"
#include "block.h"
#include "chunk.h"
#include "elim_calls.h"
#include "elim_if_stmts.h"
#include "elim_labels.h"
#include "elim_ret_stmts.h"
#include "elim_scopes.h"
#include "elim_vars_proc.h"
#include "entry_exit.h"
#include "flatten.h"
#include "nex_lang.h"
#include "parse_cyk.h"
#include "post_processing.h"
#include "procedure.h"
#include "pseudo_assembly.h"
#include "reg.h"
#include "utils.h"
#include "word.h"
#include "write_file.h"
#include "call.h"
#include "grammar.h"

struct Code;
struct TypedProcedure;
struct Variable;

static uint32_t TERMINATION_PC = 0b11111110111000011101111010101101;
static std::string file_name("test_max.bin");

std::vector<std::shared_ptr<Code>> compile_test(std::string input) {
    Grammar grammar = make_grammar();
    auto tokens = scan(input);
    auto ast_node = parse_cyk(tokens, grammar);

    if (!ast_node) {
        std::cerr << "Failed to parse!" << std::endl;
        exit(1);
    }

    std::vector<std::shared_ptr<Code>> static_data;
    auto typed_ids = generate(ast_node.value(), static_data);
    std::vector<std::shared_ptr<Procedure>> procedures;
    for (auto typed_id : typed_ids) {
        if (auto typed_proc =
                std::dynamic_pointer_cast<TypedProcedure>(typed_id)) {
            procedures.push_back(typed_proc->procedure);
        } else {
            std::cerr << "TODO!" << std::endl;
            exit(1);
        }
    }

    std::shared_ptr<Procedure> main_proc;
    for (auto proc : procedures) {
        if (proc->name == "main") {
            main_proc = proc;
        }
    }

    std::map<std::shared_ptr<Procedure>, std::shared_ptr<Chunk>> param_chunks;
    for (auto proc : procedures) {
        param_chunks[proc] = std::make_shared<Chunk>(proc->parameters);
    }

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

        ElimRetStmts elim_ret_stmts {proc->end_label};
        proc->code = proc->code->accept(elim_ret_stmts);

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
    return program3;
}

TEST_CASE("Test code gen", "[post_processing]") {
    std::string input =
        "fn main(x: i32, y: i32) -> i32 {"
        "   let result: i32 = x + y;"
        "   return result;"
        "}";

    auto program = compile_test(input);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 5, 7)) == 12);
}

TEST_CASE("Test two functions", "[post_processing]") {
    std::string input =
        "fn add(x: i32, y: i32) -> i32 {"
        "   return x + y;"
        "}"
        "fn main(x: i32, y: i32) -> i32 {"
        "   return add(x, y);"
        "}";

    auto program = compile_test(input);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 5, 7)) == 12);
}

TEST_CASE("Test max func", "[post_processing]") {
    std::string input =
        "fn max(x: i32, y: i32) -> i32 {"
        "   let result: i32 = 0;"
        "   if (x > y) {"
        "       result = x;"
        "   } else {"
        "       result = y;"
        "   }"
        "   return result;"
        "}"
        ""
        "fn main(x: i32, y: i32) -> i32 {"
        "   let z: i32 = max(x, y);"
        "   return z;"
        "}";

    auto program = compile_test(input);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 5, 7)) == 7);
}
