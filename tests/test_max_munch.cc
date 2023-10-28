

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "lang.h"
#include "scanning.h"

TEST_CASE("Test scanning various symbols", "[scanning]") {
    DFA dfa = make_dfa();
    std::vector<Token> tokens;

    tokens = maximal_munch_scan("->", dfa);
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {Token {"ARROW", "->"}})
    );
}

TEST_CASE("Test scanning numbers", "[scanning]") {
    DFA dfa = make_dfa();
    std::vector<Token> tokens;

    tokens = maximal_munch_scan("1 2 3 15 n72 0 51243 2", dfa);
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {
            Token {"NUM", "1"},
            Token {"WHITESPACE", " "},
            Token {"NUM", "2"},
            Token {"WHITESPACE", " "},
            Token {"NUM", "3"},
            Token {"WHITESPACE", " "},
            Token {"NUM", "15"},
            Token {"WHITESPACE", " "},
            Token {"ID", "n72"},
            Token {"WHITESPACE", " "},
            Token {"ZERO", "0"},
            Token {"WHITESPACE", " "},
            Token {"NUM", "51243"},
            Token {"WHITESPACE", " "},
            Token {"NUM", "2"},
        })
    );
}
