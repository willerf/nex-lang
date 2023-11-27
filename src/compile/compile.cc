
#include "compile.h"

#include <stdint.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <map>
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
#include "compile_procedure.h"
#include "define_label.h"
#include "elim_labels.h"
#include "extract_symbols.h"
#include "flatten.h"
#include "heap.h"
#include "label.h"
#include "nex_lang_parsing.h"
#include "nex_lang_scanning.h"
#include "nl_lib.h"
#include "post_processing.h"
#include "procedure.h"
#include "program_context.h"
#include "pseudo_assembly.h"
#include "reg.h"
#include "symbol_table.h"
#include "typed_procedure.h"
#include "use_label.h"
#include "word.h"

struct Variable;

static uint32_t TERMINATION_PC = 0b11111110111000011101111010101101;

std::vector<std::shared_ptr<Code>>
compile(std::vector<std::string> input_file_paths) {
    std::vector<std::pair<std::string, ASTNode>> modules;
    ProgramContext program_context;

    // add in heap as module
    std::shared_ptr<Label> heap_start_label =
        std::make_shared<Label>("heap start");
    std::shared_ptr<Code> heap_start =
        make_block({make_lis(Reg::Result), make_use(heap_start_label)});
    std::shared_ptr<TypedProcedure> heap_allocate =
        make_heap_allocate(heap_start);
    std::shared_ptr<TypedProcedure> heap_free = make_heap_free(heap_start);
    SymbolTable heap_module;
    heap_module[{heap_allocate_id, {}}] = heap_allocate;
    heap_module[{heap_free_id, {}}] = heap_free;

    program_context.module_table[heap_module_id] = heap_module;

    std::vector<std::string> import_list;
    // lex, parse and extract symbols from all provided files
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
            auto tokens = scan(input);
            auto ast_node = parse(tokens);
            auto result_list = extract_symbols(ast_node, program_context);
            import_list.insert(
                import_list.end(),
                result_list.begin(),
                result_list.end()
            );
            modules.push_back({input_file_path, ast_node});
        } catch (CompileError& compile_error) {
            compile_error.input_file_path = input_file_path;
            throw;
        }
    }

    for (size_t i = 0; i < import_list.size(); ++i) {
        std::string import_name = import_list.at(i);
        nl_lib_import(import_name, import_list, program_context, modules);
    }

    // generated intermediete code of all procedures
    std::vector<std::shared_ptr<Procedure>> procedures;
    std::vector<std::shared_ptr<Code>> static_data;
    for (auto& [input_file_path, ast_node] : modules) {
        std::vector<std::shared_ptr<TypedProcedure>> typed_procs;
        try {
            typed_procs = generate(ast_node, static_data, program_context);
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

    // add program entry point
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

    // compile down intermediete representations into machine code
    for (auto proc : procedures) {
        compile_procedure(proc, param_chunks);
    }

    for (auto proc : procedures) {
        all_code.push_back(proc->code);
    }

    auto program = make_block(
        {make_block(all_code),
         make_block(static_data),
         make_define(heap_start_label)}
    );

    Flatten flatten;
    program->accept(flatten);

    auto program2 = flatten.get();

    auto program3 = elim_labels(program2);
    return program3;
}
