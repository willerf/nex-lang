
cmake_minimum_required(VERSION 3.21)
project(nex-lang)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(compiler_lib STATIC
    src/compile.cc
    src/exceptions/compile_error.cc
    src/exceptions/duplicate_symbol_error.cc
    src/exceptions/symbol_not_found_error.cc
    src/exceptions/type_mismatch_error.cc
    src/exceptions/scanning_error.cc
    src/exceptions/parsing_error.cc
    src/lex_parse/ast_node.cc
    src/lex_parse/memo_map.cc
    src/lex_parse/parse_cyk.cc
    src/lex_parse/parse_earley.cc
    src/lex_parse/scanning.cc
    src/memory_management/chunk.cc
    src/memory_management/stack.cc
    src/nex_lang/nex_lang.cc
    src/nex_lang/post_processing/post_processing.cc
    src/nex_lang/post_processing/symbol_table.cc
    src/nex_lang/post_processing/visit_args.cc
    src/nex_lang/post_processing/visit_expr.cc
    src/nex_lang/post_processing/visit_fns.cc
    src/nex_lang/post_processing/visit_imports.cc
    src/nex_lang/post_processing/visit_params.cc
    src/nex_lang/post_processing/visit_s.cc
    src/nex_lang/post_processing/visit_stmts.cc
    src/nex_lang/post_processing/visit_type.cc
    src/nex_lang/post_processing/visit_vardef.cc
    src/nex_lang/post_processing/extract_symbols.cc
    src/nex_lang/post_processing/extract_s.cc
    src/nex_lang/post_processing/extract_fns.cc
    src/nex_lang/symbols/typed_procedure.cc
    src/nex_lang/symbols/typed_variable.cc
    src/nex_lang/types/nl_type.cc
    src/nex_lang/types/nl_type_none.cc
    src/nex_lang/types/nl_type_bool.cc
    src/nex_lang/types/nl_type_i32.cc
    src/nex_lang/types/nl_type_char.cc
    src/nex_lang/types/nl_type_ptr.cc
    src/program_representation/assembly.cc
    src/program_representation/code_builders/bin_op.cc
    src/program_representation/code_builders/operators.cc
    src/program_representation/code_builders/while_loop.cc
    src/program_representation/code_structures/beq_label.cc
    src/program_representation/code_structures/block.cc
    src/program_representation/code_structures/bne_label.cc
    src/program_representation/code_structures/call.cc
    src/program_representation/code_structures/define_label.cc
    src/program_representation/code_structures/if_stmt.cc
    src/program_representation/code_structures/ret_stmt.cc
    src/program_representation/code_structures/scope.cc
    src/program_representation/code_structures/use_label.cc
    src/program_representation/code_structures/var_access.cc
    src/program_representation/code_structures/word.cc
    src/program_representation/label.cc
    src/program_representation/procedure.cc
    src/program_representation/pseudo_assembly.cc
    src/program_representation/variable.cc
    src/transformations/elim_calls.cc
    src/transformations/elim_if_stmts.cc
    src/transformations/elim_labels.cc
    src/transformations/elim_ret_stmts.cc
    src/transformations/elim_scopes.cc
    src/transformations/elim_vars.cc
    src/transformations/elim_vars_proc.cc
    src/transformations/entry_exit.cc
    src/transformations/flatten.cc
    src/transformations/print.cc
    src/transformations/visitor.cc
    src/transformations/write_file.cc
    src/utils/reg.cc
    src/utils/state.cc
    src/utils/token.cc
)

target_include_directories(compiler_lib PUBLIC
    src
    src/exceptions
    src/lex_parse
    src/memory_management
    src/nex_lang
    src/nex_lang/post_processing
    src/nex_lang/symbols
    src/nex_lang/types
    src/program_representation
    src/program_representation/code_builders
    src/program_representation/code_structures
    src/transformations
    src/utils
)

add_executable(cnl src/main.cc)
target_link_libraries(cnl compiler_lib)

add_subdirectory(examples)
add_subdirectory(tests)

