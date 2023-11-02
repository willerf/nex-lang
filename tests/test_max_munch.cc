

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "nex_lang.h"
#include "scanning.h"

TEST_CASE("Test scanning various symbols", "[scanning]") {
    DFA dfa = make_dfa();
    std::vector<Token> tokens;

    tokens = maximal_munch_scan("->", dfa);
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {
            Token {Terminal::ARROW, "->"}})
    );
}

TEST_CASE("Test scanning numbers", "[scanning]") {
    DFA dfa = make_dfa();
    std::vector<Token> tokens;

    tokens = maximal_munch_scan("1 2 3 15 n72 0 51243 2", dfa);
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {
            Token {Terminal::NUM, "1"},
            Token {Terminal::WHITESPACE, " "},
            Token {Terminal::NUM, "2"},
            Token {Terminal::WHITESPACE, " "},
            Token {Terminal::NUM, "3"},
            Token {Terminal::WHITESPACE, " "},
            Token {Terminal::NUM, "15"},
            Token {Terminal::WHITESPACE, " "},
            Token {Terminal::ID, "n72"},
            Token {Terminal::WHITESPACE, " "},
            Token {Terminal::ZERO, "0"},
            Token {Terminal::WHITESPACE, " "},
            Token {Terminal::NUM, "51243"},
            Token {Terminal::WHITESPACE, " "},
            Token {Terminal::NUM, "2"},
        })
    );
}
