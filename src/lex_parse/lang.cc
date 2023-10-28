#include "lang.h"

#include <iostream>

#include "scanning.h"

static std::map<char, State> one_char_symbols = {
    {' ', "WHITESPACE"},  {'\t', "WHITESPACE"}, {'\n', "WHITESPACE"},
    {'\r', "WHITESPACE"}, {'0', "ZERO"},        {'<', "LT"},
    {'>', "GT"},          {'!', "NOT"},         {'=', "ASSIGN"},
    {'+', "PLUS"},        {'-', "MINUS"},       {'*', "STAR"},
    {'/', "SLASH"},       {'%', "PCT"},         {'(', "LPAREN"},
    {')', "RPAREN"},      {'{', "LBRACE"},      {'}', "RBRACE"},
    {',', "COMMA"},       {';', "SEMI"},        {':', "COLON"},
    {'|', "PIPE"},        {'&', "AMPERSAND"},
};

static std::map<std::string, State> two_char_symbols = {
    {"==", "EQ"},
    {"!=", "NE"},
    {"<=", "LE"},
    {">=", "GE"},
    {"||", "OR"},
    {"&&", "AND"},
    {"->", "ARROW"},
    {"//", "COMMENT"},
};

static std::optional<State> transition_func(State curr_state, char c) {
    if (curr_state == "start") {
        if (one_char_symbols.count(c)) {
            return one_char_symbols[c];
        }

        if (isalpha(c)) {
            return "ID";
        }

        if (isdigit(c)) {
            return "NUM";
        }
    }

    if (curr_state == "ASSIGN" && c == '=') {
        return "EQ";
    }

    if (curr_state == "NOT" && c == '=') {
        return "NE";
    }

    if (curr_state == "LT" && c == '=') {
        return "LE";
    }

    if (curr_state == "GT" && c == '=') {
        return "GE";
    }

    if (curr_state == "PIPE" && c == '|') {
        return "OR";
    }

    if (curr_state == "AMPERSAND" && c == '&') {
        return "AND";
    }

    if (curr_state == "MINUS" && c == '>') {
        return "ARROW";
    }

    if (curr_state == "SLASH" && c == '/') {
        return "COMMENT";
    }

    if (curr_state == "COMMENT" && (c != '\n' || c != '\r')) {
        return "COMMENT";
    }

    if (curr_state == "NUM" && isdigit(c)) {
        return "NUM";
    }

    if (curr_state == "ID" && std::isalnum(c)) {
        return "ID";
    }

    return std::nullopt;
}

DFA make_dfa() {
    std::string alphabet_str =
        "<>=+-*/%(){},;:! \t\n\rabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::set<char> alphabet(alphabet_str.begin(), alphabet_str.end());
    State init_state = "start";
    std::set<State> valid_states = {
        "start",
    };

    std::set<State> accepting = {
        "ID",
        "NUM",
    };

    for (const auto& [key, value] : one_char_symbols) {
        accepting.insert(value);
    }

    for (const auto& [key, value] : two_char_symbols) {
        accepting.insert(value);
    }

    valid_states.insert(accepting.begin(), accepting.end());

    return DFA {
        .alphabet = alphabet,
        .init_state = "start",
        .valid_states = valid_states,
        .accepting = accepting,
        .transition = transition_func,
    };
}

static std::map<std::string, State> keywords = {
    {"fn", "FN"},
    {"let", "LET"},
    {"if", "IF"},
    {"else", "ELSE"},
    {"return", "RET"},
    {"i32", "I32"},
};

std::vector<Token> scan(std::string_view input) {
    DFA dfa = make_dfa();

    std::vector<Token> tokens = maximal_munch_scan(input, dfa);
    for (Token& token : tokens) {
        if (keywords.count(token.lexeme)) {
            token.kind = keywords[token.lexeme];
        } else if (token.kind == "ZERO") {
            token.kind = "NUM";
        }
    }

    std::set<State> sep_set1 = {"FN", "LET", "IF", "ELSE", "I32", "ID", "NUM"};
    std::set<State> sep_set2 =
        {"EQ", "NE", "LT", "LE", "GT", "GE", "OR", "AND", "ASSIGN", "ARROW"};
    bool prev_set1 = false;
    bool prev_set2 = false;

    std::vector<Token> result;
    for (Token& token : tokens) {
        if (sep_set1.count(token.kind)) {
            if (prev_set1) {
                std::cerr << "Invalid consecutive keywords!";
                exit(1);
            }
            prev_set1 = true;
        } else {
            prev_set1 = false;
        }

        if (sep_set2.count(token.kind)) {
            if (prev_set2) {
                std::cerr << "Invalid consecutive symbols!";
                exit(1);
            }
            prev_set2 = true;
        } else {
            prev_set2 = false;
        }

        if (token.kind != "WHITESPACE" && token.kind != "COMMENT") {
            result.push_back(token);
        }
    }

    result.insert(result.begin(), Token {"BOF", ""});
    result.push_back(Token {"EOF", ""});

    return result;
}

