

#include <catch2/catch_test_macros.hpp>
#include <span>
#include <string>

#include "nex_lang_parsing.h"
#include "nex_lang_scanning.h"

TEST_CASE("Test parsing lang", "[parser]") {
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
        "fn main() -> i32 {"
        "   let z: i32 = max(5, 12);"
        "}";
    auto tokens = scan(input);
    auto ast_node = parse(tokens);
}
