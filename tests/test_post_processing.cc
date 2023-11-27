

#include <catch2/catch_test_macros.hpp>
#include <string>

#include "utils.h"
#include "write_file.h"

static std::string file_name("test_post_processing.bin");

TEST_CASE("code gen", "[post_processing]") {
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

TEST_CASE("two functions", "[post_processing]") {
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

TEST_CASE("max func", "[post_processing]") {
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
