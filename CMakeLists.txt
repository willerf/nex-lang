
cmake_minimum_required(VERSION 3.21)
project(cpp-compiler-v2)
set(CMAKE_CXX_STANDARD 20)

add_library(compiler_lib STATIC
    src/memory_management/chunk.cc
    src/memory_management/stack.cc
    src/program_representation/assembly.cc
    src/program_representation/pseudo_assembly.cc
    src/program_representation/code_builders/bin_op.cc
    src/program_representation/code_builders/comparators.cc
    src/program_representation/code_builders/operators.cc
    src/program_representation/code_builders/while_loop.cc
    src/program_representation/code_structures/block.cc
    src/program_representation/code_structures/beq_label.cc
    src/program_representation/code_structures/bne_label.cc
    src/program_representation/code_structures/define_label.cc
    src/program_representation/code_structures/if_stmt.cc
    src/program_representation/code_structures/scope.cc
    src/program_representation/code_structures/call.cc
    src/program_representation/code_structures/use_label.cc
    src/program_representation/code_structures/var_access.cc
    src/program_representation/code_structures/word.cc
    src/program_representation/label.cc
    src/program_representation/variable.cc
    src/program_representation/procedure.cc
    src/transformations/elim_if_stmts.cc
    src/transformations/elim_labels.cc
    src/transformations/elim_scopes.cc
    src/transformations/elim_vars.cc
    src/transformations/elim_calls.cc
    src/transformations/flatten.cc
    src/transformations/print.cc
    src/transformations/visitor.cc
    src/transformations/write_file.cc
    src/utils/reg.cc
)

target_include_directories(compiler_lib PUBLIC
    src
    src/memory_management
    src/program_representation
    src/program_representation/code_structures
    src/program_representation/code_builders
    src/transformations
    src/utils
)

add_executable(main src/main.cc)
target_link_libraries(main compiler_lib)

add_subdirectory(examples)
add_subdirectory(tests)

