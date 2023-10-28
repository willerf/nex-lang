

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "lang.h"
#include "scanning.h"

TEST_CASE("Test scanning keywords", "[scanning]") {
    DFA dfa = make_dfa();
    std::vector<Token> tokens;

    tokens = scan("fn", dfa);
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {Token {"FN", "fn"}})
    );

    tokens = scan("let", dfa);
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {Token {"LET", "let"}})
    );

    tokens = scan("if", dfa);
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {Token {"IF", "if"}})
    );

    tokens = scan("else", dfa);
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {Token {"ELSE", "else"}})
    );

    tokens = scan("i32", dfa);
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {Token {"I32", "i32"}})
    );
}

TEST_CASE("Test scanning various symbols", "[scanning]") {
    DFA dfa = make_dfa();
    std::vector<Token> tokens;

    tokens = scan("->", dfa);
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {Token {"ARROW", "->"}})
    );
}

TEST_CASE("Test scanning simple program", "[scanning]") {
    DFA dfa = make_dfa();
    std::vector<Token> tokens;

    std::string program = "fn add(x: i32, y: i32) -> i32 {x + y}";
    tokens = scan(program, dfa);
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {
            Token {"FN", "fn"},        Token {"WHITESPACE", " "},
            Token {"ID", "add"},       Token {"LPAREN", "("},
            Token {"ID", "x"},         Token {"COLON", ":"},
            Token {"WHITESPACE", " "}, Token {"I32", "i32"},
            Token {"COMMA", ","},      Token {"WHITESPACE", " "},
            Token {"ID", "y"},         Token {"COLON", ":"},
            Token {"WHITESPACE", " "}, Token {"I32", "i32"},
            Token {"RPAREN", ")"},     Token {"WHITESPACE", " "},
            Token {"ARROW", "->"},     Token {"WHITESPACE", " "},
            Token {"I32", "i32"},      Token {"WHITESPACE", " "},
            Token {"LBRACE", "{"},     Token {"ID", "x"},
            Token {"WHITESPACE", " "}, Token {"PLUS", "+"},
            Token {"WHITESPACE", " "}, Token {"ID", "y"},
            Token {"RBRACE", "}"},
        })
    );
}
