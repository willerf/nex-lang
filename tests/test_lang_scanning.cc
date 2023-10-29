

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "lang.h"

TEST_CASE("Test scanning keywords", "[lang]") {
    std::vector<Token> tokens;

    tokens = scan("fn");
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {
            Token {"BOF"
                   ""},
            Token {"FN", "fn"},
            Token {"EOF", ""},
        })
    );

    tokens = scan("let");
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {
            Token {"BOF"
                   ""},
            Token {"LET", "let"},
            Token {"EOF", ""},
        })
    );

    tokens = scan("if");
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {
            Token {"BOF"
                   ""},
            Token {"IF", "if"},
            Token {"EOF", ""},
        })
    );

    tokens = scan("else");
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {
            Token {"BOF"
                   ""},
            Token {"ELSE", "else"},
            Token {"EOF", ""},
        })
    );

    tokens = scan("i32");
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {
            Token {"BOF"
                   ""},
            Token {"I32", "i32"},
            Token {"EOF", ""},
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
            Token {"BOF", ""},     Token {"FN", "fn"},   Token {"ID", "add"},
            Token {"LPAREN", "("}, Token {"ID", "x"},    Token {"COLON", ":"},
            Token {"I32", "i32"},  Token {"COMMA", ","}, Token {"ID", "y"},
            Token {"COLON", ":"},  Token {"I32", "i32"}, Token {"RPAREN", ")"},
            Token {"ARROW", "->"}, Token {"I32", "i32"}, Token {"LBRACE", "{"},
            Token {"ID", "x"},     Token {"PLUS", "+"},  Token {"ID", "y"},
            Token {"RBRACE", "}"}, Token {"EOF", ""},
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
            Token {"BOF", ""},       Token {"FN", "fn"},
            Token {"ID", "max"},     Token {"LPAREN", "("},
            Token {"ID", "x"},       Token {"COLON", ":"},
            Token {"I32", "i32"},    Token {"COMMA", ","},
            Token {"ID", "y"},       Token {"COLON", ":"},
            Token {"I32", "i32"},    Token {"RPAREN", ")"},
            Token {"ARROW", "->"},   Token {"I32", "i32"},
            Token {"LBRACE", "{"},   Token {"IF", "if"},
            Token {"LPAREN", "("},   Token {"ID", "x"},
            Token {"GT", ">"},       Token {"ID", "y"},
            Token {"RPAREN", ")"},   Token {"LBRACE", "{"},
            Token {"RET", "return"}, Token {"ID", "x"},
            Token {"SEMI", ";"},     Token {"ELSE", "else"},
            Token {"LBRACE", "{"},   Token {"RET", "return"},
            Token {"ID", "y"},       Token {"SEMI", ";"},
            Token {"RBRACE", "}"},   Token {"RBRACE", "}"},
            Token {"EOF", ""},
        })
    );
}
