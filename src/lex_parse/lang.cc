
#include "lang.h"

static std::set<std::string> to_id_states = {
    "L",
    "LE",
    "LET",
    "F",
    "FN",
    "I",
    "IF",
    "E",
    "EL",
    "ELS",
    "ELSE",
    "I3",
    "I32",
    "ID"};

static std::map<char, std::string> symbols = {
    {' ', "WHITESPACE"},  {'\t', "WHITESPACE"}, {'\n', "WHITESPACE"},
    {'\r', "WHITESPACE"}, {'0', "ZERO"},        {'<', "LT"},
    {'>', "GT"},          {'!', "NOT"},         {'=', "ASSIGN"},
    {'+', "PLUS"},        {'-', "MINUS"},       {'*', "STAR"},
    {'/', "SLASH"},       {'%', "PCT"},         {'(', "LPAREN"},
    {')', "RPAREN"},      {'{', "LBRACE"},      {'}', "RBRACE"},
    {',', "COMMA"},       {';', "SEMI"},        {':', "COLON"},
};

static std::map<std::string, std::string> two_char_symbols = {
    {"==", "EQ"},
    {"!=", "NE"},
    {"<=", "LE"},
    {">=", "GE"},
    {"->", "ARROW"},
    {"//", "COMMENT"},
};

static std::optional<State> transition_func(State curr_state, char c) {
    if (curr_state == "start") {
        if (symbols.count(c)) {
            return symbols[c];
        }

        switch (c) {
            case 'l':
                return "L";
            case 'f':
                return "F";
            case 'i':
                return "I";
            case 'e':
                return "E";
        }

        if (isalpha(c)) {
            return "ID";
        }
    }

    if (curr_state == "L" && c == 'e') {
        return "LE";
    }

    if (curr_state == "LE" && c == 't') {
        return "LET";
    }

    if (curr_state == "F" && c == 'n') {
        return "FN";
    }

    if (curr_state == "I") {
        switch (c) {
            case 'f':
                return "IF";
            case '3':
                return "I3";
        }
    }

    if (curr_state == "E" && c == 'l') {
        return "EL";
    }

    if (curr_state == "EL" && c == 's') {
        return "ELS";
    }

    if (curr_state == "ELS" && c == 'e') {
        return "ELSE";
    }

    if (curr_state == "I3" && c == '2') {
        return "I32";
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

    if (curr_state == "MINUS" && c == '>') {
        return "ARROW";
    }

    if (curr_state == "SLASH" && c == '/') {
        return "COMMENT";
    }

    // if all keywords fail
    if (to_id_states.count(curr_state) && std::isalnum(c)) {
        return "ID";
    }

    return std::nullopt;
}

DFA make_dfa() {
    std::string alphabet_str =
        "<>=+-*/%(){},;:! \t\n\rabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::set<char> alphabet(alphabet_str.begin(), alphabet_str.end());
    std::string init_state = "start";
    std::set<std::string> valid_states = {
        "start",
    };

    std::set<std::string> accepting = {
        "LET",
        "FN",
        "IF",
        "ELSE",
        "I32",
        "ID",
        "ARROW",
    };

    for (const auto& [key, value] : symbols) {
        accepting.insert(value);
    }

    valid_states.insert(accepting.begin(), accepting.end());
    valid_states.insert(to_id_states.begin(), to_id_states.end());

    return DFA {
        .alphabet = alphabet,
        .init_state = "start",
        .valid_states = valid_states,
        .accepting = accepting,
        .transition = transition_func,
    };
}
