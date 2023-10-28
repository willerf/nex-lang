
#include "scanning.h"

#include <iostream>

Token scan_one(std::string_view input, DFA& dfa) {
    State curr_state = dfa.init_state;
    std::optional<std::pair<State, std::string_view>> last_accepting =
        std::nullopt;

    for (size_t i = 0; i < input.length(); i++) {
        char c = input.at(i);
        std::optional<State> next_state = std::nullopt;
        if (dfa.alphabet.count(c)) {
            next_state = dfa.transition(curr_state, c);
        }

        if (next_state) {
            curr_state = next_state.value();
            if (dfa.accepting.count(curr_state)) {
                last_accepting = {curr_state, input.substr(0, i + 1)};
            }
        } else {
            if (last_accepting) {
                return Token {
                    last_accepting.value().first,
                    std::string(last_accepting.value().second)};
            } else {
                std::cerr << "Failed to scan, no token found!" << std::endl;
                std::cerr << input << std::endl;
                exit(1);
            }
        }
    }
    if (last_accepting) {
        return Token {
            last_accepting.value().first,
            std::string(last_accepting.value().second)};
    } else {
        std::cerr << "Failed to scan, no token found!" << std::endl;
        std::cerr << input << std::endl;
        exit(1);
    }
}

std::vector<Token> maximal_munch_scan(std::string_view input, DFA& dfa) {
    std::vector<Token> tokens;
    std::string_view rem = input;
    while (rem.length() > 0) {
        Token token = scan_one(rem, dfa);
        rem.remove_prefix(token.lexeme.length());
        tokens.push_back(token);
    }
    return tokens;
}
