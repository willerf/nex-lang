
#include "utils.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <span>
#include <sstream>
#include <string>

#include "assembly.h"
#include "block.h"
#include "call.h"
#include "chunk.h"
#include "elim_calls.h"
#include "elim_if_stmts.h"
#include "elim_labels.h"
#include "elim_ret_stmts.h"
#include "elim_scopes.h"
#include "elim_vars_proc.h"
#include "entry_exit.h"
#include "extract_symbols.h"
#include "flatten.h"
#include "grammar.h"
#include "module_table.h"
#include "nex_lang.h"
#include "parse_earley.h"
#include "post_processing.h"
#include "procedure.h"
#include "pseudo_assembly.h"
#include "reg.h"
#include "word.h"

struct TypedProcedure;
struct Variable;

static uint32_t TERMINATION_PC = 0b11111110111000011101111010101101;

std::vector<uint32_t> word_to_uint(std::vector<std::shared_ptr<Code>> program) {
    std::vector<uint32_t> result;
    for (auto code : program) {
        if (auto word = std::dynamic_pointer_cast<Word>(code)) {
            result.push_back(word->bits);
        }
    }
    return result;
}

std::string emulate(std::string file_name, int32_t input1, int32_t input2) {
    const std::string emulator_path(EMULATOR_PATH);
    const std::string output_file("test_output.txt");

    std::system((emulator_path + " " + file_name + " " + std::to_string(input1)
                 + " " + std::to_string(input2) + " > " + output_file)
                    .c_str());

    std::ifstream file(output_file);
    if (!file) {
        std::cerr << "Output file not found!" << std::endl;
        exit(1);
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

std::vector<std::shared_ptr<Code>> compile_test(std::string input) {
    Grammar grammar = make_grammar();
    auto tokens = scan(input);
    auto ast_node = parse_earley(tokens, grammar);

    if (!ast_node) {
        std::cerr << "Failed to parse!" << std::endl;
        exit(1);
    }

    ModuleTable module_table;
    extract_symbols(ast_node.value(), module_table);

    std::vector<std::shared_ptr<Code>> static_data;
    auto typed_ids = generate(ast_node.value(), static_data, module_table);
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
