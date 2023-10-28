
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <iostream>

#include "scanning.h"

TEST_CASE("Test simple scanning", "[scanning]") {
    auto transition_func = [](State curr_state,
                              char c) -> std::optional<State> {
        if (curr_state == "start" && c == 'c') {
            return "C";
        }
        if (curr_state == "C" && c == 'a') {
            return "CA";
        }
        if (curr_state == "CA" && c == 'b') {
            return "CAB";
        }
        if (curr_state == "start" && c == 'b') {
            return "B";
        }
        if (curr_state == "B" && c == 'a') {
            return "BA";
        }
        if (curr_state == "BA" && c == 'd') {
            return "BAD";
        }
        return std::nullopt;
    };

    DFA dfa {
        .alphabet = {'a', 'b', 'c', 'd'},
        .init_state = "start",
        .valid_states = {"start", "C", "CA", "CAB", "B", "BA", "BAD"},
        .accepting = {"CAB", "CA", "BAD"},
        .transition = transition_func};
    std::vector<Token> tokens;

    tokens = scan("cab", dfa);
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {Token {"CAB", "cab"}})
    );

    tokens = scan("cabcab", dfa);
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(
            std::vector<Token> {Token {"CAB", "cab"}, Token {"CAB", "cab"}}
        )
    );

    tokens = scan("cabbad", dfa);
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(
            std::vector<Token> {Token {"CAB", "cab"}, Token {"BAD", "bad"}}
        )
    );

    tokens = scan("", dfa);
    REQUIRE_THAT(tokens, Catch::Matchers::Equals(std::vector<Token> {}));

    tokens = scan("cacacabbadbadca", dfa);
    REQUIRE_THAT(
        tokens,
        Catch::Matchers::Equals(std::vector<Token> {
            Token {"CA", "ca"},
            Token {"CA", "ca"},
            Token {"CAB", "cab"},
            Token {"BAD", "bad"},
            Token {"BAD", "bad"},
            Token {"CA", "ca"}})
    );
}
