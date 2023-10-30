
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <iostream>

#include "parsing.h"

Grammar create_simple_grammar() {
    Grammar grammar;

    State E = "E";
    State T = "T";
    State F = "F";
    State PLUS = "PLUS";
    State STAR = "STAR";
    State ID = "ID";
    State LPAREN = "LPAREN";
    State RPAREN = "RPAREN";

    grammar.non_terminals = {E, T, F};
    grammar.terminals = {PLUS, STAR, ID, LPAREN, RPAREN};
    grammar.start = E;

    grammar.productions[E].push_back({E, {E, PLUS, T}});
    grammar.productions[E].push_back({E, {T}});
    grammar.productions[T].push_back({T, {T, STAR, F}});
    grammar.productions[T].push_back({T, {F}});
    grammar.productions[F].push_back({F, {ID}});
    grammar.productions[F].push_back({F, {LPAREN, E, RPAREN}});

    return grammar;
}

TEST_CASE("Test parsing with simple arithmetic grammar", "[parser]") {
    Grammar grammar = create_simple_grammar();

    std::vector<Token> tokens = {
        Token {"ID", "a"},
        Token {"PLUS", "+"},
        Token {"ID", "b"},
        Token {"STAR", "*"},
        Token {"LPAREN", "("},
        Token {"ID", "c"},
        Token {"PLUS", "+"},
        Token {"ID", "d"},
        Token {"RPAREN", ")"},
    };

    auto ast_node = parse_cyk(tokens, grammar);

    REQUIRE(ast_node->token.kind == "E");
    // std::cout << ast_node->to_string(0) << std::endl;
}
