
#include "scanning.h"

#include <iostream>

#include "scanning_error.h"

Token scan_one(std::string_view input, DFA& dfa, size_t line_no) {
    Terminal curr_state = dfa.init_state;
    std::optional<std::pair<Terminal, std::string_view>> last_accepting =
        std::nullopt;

    for (size_t i = 0; i < input.length(); i++) {
        char c = input.at(i);
        std::optional<Terminal> next_state = std::nullopt;
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
                throw ScanningError(line_no);
            }
        }
    }
    if (last_accepting) {
        return Token {
            last_accepting.value().first,
            std::string(last_accepting.value().second)};
    } else {
        throw ScanningError(line_no);
    }
}

std::vector<Token> maximal_munch_scan(std::string_view input, DFA& dfa) {
    std::vector<Token> tokens;
    std::string_view rem = input;
    size_t line_no = 1;
    while (rem.length() > 0) {
        Token token = scan_one(rem, dfa, line_no);
        rem.remove_prefix(token.lexeme.length());
        token.line_no = line_no;
        if (token.kind == Terminal::NEWLINE) {
            ++line_no;
        }
        tokens.push_back(token);
    }
    return tokens;
}
