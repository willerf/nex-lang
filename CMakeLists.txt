
cmake_minimum_required(VERSION 3.21)
project(cpp-compiler-v2)
set(CMAKE_CXX_STANDARD 20)

add_library(compiler_lib STATIC
    src/memory_management/chunk.cc
    src/memory_management/stack.cc
    src/program_representation/assembly.cc
    src/program_representation/bin_op.cc
    src/program_representation/code_structures/block.cc
    src/program_representation/code_structures/beq_label.cc
    src/program_representation/code_structures/bne_label.cc
    src/program_representation/code_structures/define_label.cc
    src/program_representation/code_structures/if_stmt.cc
    src/program_representation/code_structures/scope.cc
    src/program_representation/code_structures/use_label.cc
    src/program_representation/code_structures/var_access.cc
    src/program_representation/code_structures/word.cc
    src/program_representation/label.cc
    src/program_representation/variable.cc
    src/transformations/elim_if_stmts.cc
    src/transformations/elim_labels.cc
    src/transformations/elim_scopes.cc
    src/transformations/elim_vars.cc
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
    src/transformations
    src/utils
)

add_executable(main src/main.cc)
target_link_libraries(main compiler_lib)

add_subdirectory(examples)
add_subdirectory(tests)

add_custom_command(
    OUTPUT emulator_executable
    COMMAND cargo build --release
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/emulation
    COMMENT "Building Rust emulator executable"
)

add_custom_target(RustEmulator
    DEPENDS emulator_executable
)

add_custom_target(CopyRustEmulator
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/emulation/target/release/emulation
    ${CMAKE_BINARY_DIR}/emulation
    DEPENDS RustEmulator 
)

