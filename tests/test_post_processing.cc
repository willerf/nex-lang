

#include <stdint.h>
#include <stdlib.h>

#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <vector>

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
#include "utils.h"
#include "word.h"
#include "write_file.h"

struct Code;
struct TypedProcedure;
struct Variable;

static std::string file_name("test_max.bin");

TEST_CASE("Test code gen", "[post_processing]") {
    std::string input =
        "mod main;"
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
        "mod main;"
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
        "mod main;"
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
