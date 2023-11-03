

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <iostream>

#include "nex_lang.h"
#include "parsing.h"

TEST_CASE("Test parsing lang", "[parser]") {
    Grammar grammar = make_grammar();

    std::string input =
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
    auto ast_node = parse_cyk(tokens, grammar);

    // std::cout << ast_node->to_string(0) << std::endl;
}
