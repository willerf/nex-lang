
#include "compile.h"

#include <stdint.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <span>
#include <sstream>
#include <string>
#include <utility>

#include "assembly.h"
#include "ast_node.h"
#include "block.h"
#include "call.h"
#include "chunk.h"
#include "compile_error.h"
#include "define_label.h"
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
#include "heap.h"
#include "module_table.h"
#include "nex_lang.h"
#include "parse_earley.h"
#include "post_processing.h"
#include "procedure.h"
#include "pseudo_assembly.h"
#include "reg.h"
#include "use_label.h"
#include "word.h"

struct TypedProcedure;
struct Variable;

static uint32_t TERMINATION_PC = 0b11111110111000011101111010101101;

void compile_procedure(
    std::shared_ptr<Procedure> proc,
    std::map<std::shared_ptr<Procedure>, std::shared_ptr<Chunk>> param_chunks
) {
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
    ElimVarsProc elim_vars_proc(local_vars_chunk, param_chunk, proc->param_ptr);
    proc->code = proc->code->accept(elim_vars_proc);
}

std::vector<std::shared_ptr<Code>>
compile(std::vector<std::string> input_file_paths) {
    std::vector<std::pair<std::string, ASTNode>> modules;
    ModuleTable module_table;


    std::shared_ptr<Label> heap_start_label = std::make_shared<Label>("heap start");
    std::shared_ptr<Code> heap_start = make_block({make_lis(Reg::Result), make_use(heap_start_label)});
    std::shared_ptr<Code> heap_end = int_literal(8096);
    std::shared_ptr<TypedProcedure> heap_allocate = make_heap_allocate(heap_start, heap_end);
    std::shared_ptr<TypedProcedure> heap_free = make_heap_free(heap_start);
    SymbolTable heap_module;
    heap_module["heap_allocate"] = heap_allocate;
    heap_module["heap_free"] = heap_free;
    module_table["heap"] = heap_module;

    for (std::string input_file_path : input_file_paths) {
        std::ifstream file {input_file_path};
        if (!file) {
            std::cerr << "Invalid file path: " << input_file_path << std::endl;
            exit(1);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string input = buffer.str();
        try {
            Grammar grammar = make_grammar();
            auto tokens = scan(input);
            auto opt_ast_node = parse_earley(tokens, grammar);
            if (!opt_ast_node) {
                std::cerr << "Failed to parse." << std::endl;
                exit(1);
            }
            ASTNode ast_node = opt_ast_node.value();
            extract_symbols(ast_node, module_table);
            modules.push_back({input_file_path, ast_node});
        } catch (CompileError& compile_error) {
            compile_error.input_file_path = input_file_path;
            throw;
        }
    }

    std::vector<std::shared_ptr<Procedure>> procedures;
    std::vector<std::shared_ptr<Code>> static_data;
    for (auto& [input_file_path, ast_node] : modules) {
        std::vector<std::shared_ptr<TypedProcedure>> typed_procs;
        try {
            typed_procs = generate(ast_node, static_data, module_table);
        } catch (CompileError& compile_error) {
            compile_error.input_file_path = input_file_path;
            throw;
        }
        for (auto typed_proc : typed_procs) {
            procedures.push_back(typed_proc->procedure);
        }
    }
    
    procedures.push_back(heap_allocate->procedure);
    procedures.push_back(heap_free->procedure);

    std::vector<std::shared_ptr<Code>> all_code;

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
        {init_heap(heap_start),
         make_call(main_proc, {to_expr(Reg::Input1), to_expr(Reg::Input2)}),
         make_lis(Reg::TargetPC),
         make_word(TERMINATION_PC),
         make_jr(Reg::TargetPC)}
    );
    procedures.insert(procedures.begin(), start_proc);


    for (auto proc : procedures) {
        compile_procedure(proc, param_chunks);
    }

    for (auto proc : procedures) {
        all_code.push_back(proc->code);
    }

    auto program = make_block({make_block(all_code), make_block(static_data), make_define(heap_start_label)});

    Flatten flatten;
    program->accept(flatten);

    auto program2 = flatten.get();

    auto program3 = elim_labels(program2);
    return program3;
}
