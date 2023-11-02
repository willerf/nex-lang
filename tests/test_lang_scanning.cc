

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "nex_lang.h"

TEST_CASE("Test scanning keywords", "[lang]") {
    std::vector<Token> tokens;

    tokens = scan("fn");
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {
            Token {Terminal::BOFS,
                   ""},
            Token {Terminal::FN, "fn"},
            Token {Terminal::EOFS, ""},
        })
    );

    tokens = scan("let");
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {
            Token {Terminal::BOFS,
                   ""},
            Token {Terminal::LET, "let"},
            Token {Terminal::EOFS, ""},
        })
    );

    tokens = scan("if");
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {
            Token {Terminal::BOFS,
                   ""},
            Token {Terminal::IF, "if"},
            Token {Terminal::EOFS, ""},
        })
    );

    tokens = scan("else");
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {
            Token {Terminal::BOFS,
                   ""},
            Token {Terminal::ELSE, "else"},
            Token {Terminal::EOFS, ""},
        })
    );

    tokens = scan("i32");
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {
            Token {Terminal::BOFS,
                   ""},
            Token {Terminal::I32, "i32"},
            Token {Terminal::EOFS, ""},
        })
    );
}

TEST_CASE("Test scanning simple program", "[lang]") {
    std::vector<Token> tokens;

    std::string program = "fn add(x: i32, y: i32) -> i32 {x + y}";
    tokens = scan(program);
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {
            Token {Terminal::BOFS, ""},     Token {Terminal::FN, "fn"},   Token {Terminal::ID, "add"},
            Token {Terminal::LPAREN, "("}, Token {Terminal::ID, "x"},    Token {Terminal::COLON, ":"},
            Token {Terminal::I32, "i32"},  Token {Terminal::COMMA, ","}, Token {Terminal::ID, "y"},
            Token {Terminal::COLON, ":"},  Token {Terminal::I32, "i32"}, Token {Terminal::RPAREN, ")"},
            Token {Terminal::ARROW, "->"}, Token {Terminal::I32, "i32"}, Token {Terminal::LBRACE, "{"},
            Token {Terminal::ID, "x"},     Token {Terminal::PLUS, "+"},  Token {Terminal::ID, "y"},
            Token {Terminal::RBRACE, "}"}, Token {Terminal::EOFS, ""},
        })
    );
}

TEST_CASE("Test scanning bigger program", "[scanning]") {
    std::vector<Token> tokens;

    std::string program =
        "fn max(x: i32, y: i32) -> i32 {"
        "   if (x > y) {"
        "       return x;"
        "   else {"
        "       return y;"
        "   }"
        "}";
    tokens = scan(program);
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {
            Token {Terminal::BOFS, ""},       Token {Terminal::FN, "fn"},
            Token {Terminal::ID, "max"},     Token {Terminal::LPAREN, "("},
            Token {Terminal::ID, "x"},       Token {Terminal::COLON, ":"},
            Token {Terminal::I32, "i32"},    Token {Terminal::COMMA, ","},
            Token {Terminal::ID, "y"},       Token {Terminal::COLON, ":"},
            Token {Terminal::I32, "i32"},    Token {Terminal::RPAREN, ")"},
            Token {Terminal::ARROW, "->"},   Token {Terminal::I32, "i32"},
            Token {Terminal::LBRACE, "{"},   Token {Terminal::IF, "if"},
            Token {Terminal::LPAREN, "("},   Token {Terminal::ID, "x"},
            Token {Terminal::GT, ">"},       Token {Terminal::ID, "y"},
            Token {Terminal::RPAREN, ")"},   Token {Terminal::LBRACE, "{"},
            Token {Terminal::RET, "return"}, Token {Terminal::ID, "x"},
            Token {Terminal::SEMI, ";"},     Token {Terminal::ELSE, "else"},
            Token {Terminal::LBRACE, "{"},   Token {Terminal::RET, "return"},
            Token {Terminal::ID, "y"},       Token {Terminal::SEMI, ";"},
            Token {Terminal::RBRACE, "}"},   Token {Terminal::RBRACE, "}"},
            Token {Terminal::EOFS, ""},
        })
    );
}
