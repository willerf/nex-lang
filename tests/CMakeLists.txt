
# Catch2 for unit testing
Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1
)

FetchContent_Declare(
  rust_emulator
  GIT_REPOSITORY https://github.com/willerf/mips-emulator
  GIT_TAG        v1.6
)

FetchContent_MakeAvailable(Catch2)

FetchContent_GetProperties(rust_emulator)
if(NOT rust_emulator_POPULATED)
  FetchContent_Populate(rust_emulator)
  add_custom_target(
    build_rust_emulator ALL
    COMMAND cargo build --release
    WORKING_DIRECTORY ${rust_emulator_SOURCE_DIR}
  )
endif()


file(GLOB TEST_SOURCES *.cc)

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain compiler_lib)
add_dependencies(tests build_rust_emulator)

set(EMULATOR_PATH ${rust_emulator_SOURCE_DIR}/target/release/mips_emulator)
target_compile_definitions(tests PRIVATE EMULATOR_PATH="${EMULATOR_PATH}")

