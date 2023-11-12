
#include <catch2/catch_test_macros.hpp>
#include <string>

#include "compile.h"
#include "utils.h"
#include "write_file.h"

static std::string file_name = "test_compile.bin";

TEST_CASE("Test compile simple program", "[compile]") {
    std::string input =
        "module main;"
        "fn power(base: i32, exponent: i32) -> i32 {"
        "if (exponent == 1) {"
        "    return base;"
        "}"
        "else {"
        "    return base * power(base, exponent - 1);"
        "}"
        "}"
        ""
        "fn main(x: i32, y: i32) -> i32 {"
        "    return power(x, y);"
        "}";

    auto program = compile(input);
    write_file(file_name, program);

    REQUIRE(stoi(emulate(file_name, 1, 10)) == 1);
    REQUIRE(stoi(emulate(file_name, 2, 6)) == 64);
    REQUIRE(stoi(emulate(file_name, 3, 4)) == 81);
    REQUIRE(stoi(emulate(file_name, 2, 10)) == 1024);
}
