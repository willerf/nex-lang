
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include "scanning.h"
#include <iostream>
std::optional<State> transition_func(State curr_state, char c) {
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
}

TEST_CASE("Test scanning", "[scanning]") {
    
    DFA dfa {
        .alphabet = {'a', 'b', 'c', 'd'},
        .init_state = "start",
        .valid_states = {"start", "C", "CA", "CAB", "B", "BA", "BAD"},
        .accepting = {"CAB", "CA", "BAD"},
        .transition = transition_func
    };

    std::string orig_string = "cabad";
    std::string_view input{orig_string};
    std::vector<Token> tokens = scan(input, dfa);

    for (Token t : tokens) {
        std::cout << "Lexeme: " << t.lexeme << std::endl;
    }
}




