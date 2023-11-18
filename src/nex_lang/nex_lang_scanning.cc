#include "nex_lang_scanning.h"

#include <stdlib.h>

#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>

#include "scanning.h"
#include "state.h"

static const std::map<char, Terminal> one_char_symbols = {
    {' ', Terminal::SPACE},     {'\t', Terminal::TAB},
    {'\n', Terminal::NEWLINE},  {'\r', Terminal::CARRIAGERETURN},
    {'0', Terminal::ZERO},      {'<', Terminal::LT},
    {'>', Terminal::GT},        {'!', Terminal::NOT},
    {'=', Terminal::ASSIGN},    {'+', Terminal::PLUS},
    {'-', Terminal::MINUS},     {'*', Terminal::STAR},
    {'/', Terminal::SLASH},     {'%', Terminal::PCT},
    {'(', Terminal::LPAREN},    {')', Terminal::RPAREN},
    {'{', Terminal::LBRACE},    {'}', Terminal::RBRACE},
    {',', Terminal::COMMA},     {';', Terminal::SEMI},
    {':', Terminal::COLON},     {'|', Terminal::PIPE},
    {'&', Terminal::AMPERSAND}, {'[', Terminal::LBRACKET},
    {']', Terminal::RBRACKET},
};

static const std::map<std::string, Terminal> two_char_symbols = {
    {"==", Terminal::EQ},
    {"!=", Terminal::NE},
    {"<=", Terminal::LE},
    {">=", Terminal::GE},
    {"||", Terminal::OR},
    {"&&", Terminal::AND},
    {"->", Terminal::ARROW},
    {"//", Terminal::COMMENT},
};

static std::optional<Terminal> transition_func(Terminal curr_state, char c) {
    if (curr_state == Terminal::START) {
        if (one_char_symbols.contains(c)) {
            return one_char_symbols.at(c);
        }

        if (isalpha(c)) {
            return Terminal::ID;
        }

        if (isdigit(c)) {
            return Terminal::NUM;
        }

        if (c == '\'') {
            return Terminal::CHARLITERALNF;
        }

        if (c == '\"') {
            return Terminal::STRLITERALNF;
        }
    }

    if (curr_state == Terminal::ASSIGN && c == '=') {
        return Terminal::EQ;
    }

    if (curr_state == Terminal::NOT && c == '=') {
        return Terminal::NE;
    }

    if (curr_state == Terminal::LT && c == '=') {
        return Terminal::LE;
    }

    if (curr_state == Terminal::GT && c == '=') {
        return Terminal::GE;
    }

    if (curr_state == Terminal::PIPE && c == '|') {
        return Terminal::OR;
    }

    if (curr_state == Terminal::AMPERSAND && c == '&') {
        return Terminal::AND;
    }

    if (curr_state == Terminal::MINUS && c == '>') {
        return Terminal::ARROW;
    }

    if (curr_state == Terminal::SLASH && c == '/') {
        return Terminal::COMMENT;
    }

    if (curr_state == Terminal::COMMENT && c != '\n' && c != '\r') {
        return Terminal::COMMENT;
    }

    if (curr_state == Terminal::CHARLITERALNF) {
        if (c == '\'') {
            return Terminal::CHARLITERAL;
        } else {
            return Terminal::CHARLITERALNF;
        }
    }

    if (curr_state == Terminal::STRLITERALNF) {
        if (c == '\"') {
            return Terminal::STRLITERAL;
        } else {
            return Terminal::STRLITERALNF;
        }
    }

    if (curr_state == Terminal::NUM && isdigit(c)) {
        return Terminal::NUM;
    }

    if (curr_state == Terminal::ID && (isalpha(c) || isdigit(c) || c == '_')) {
        return Terminal::ID;
    }

    return std::nullopt;
}

DFA make_nex_lang_dfa() {
    std::string alphabet_str =
        "<>=+-_*/%(){}[],;:!&| \t\n\r\'\"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::set<char> alphabet(alphabet_str.begin(), alphabet_str.end());
    Terminal init_state = Terminal::START;

    std::set<Terminal> valid_states = {
        Terminal::START,
    };

    std::set<Terminal> accepting = {
        Terminal::ID,
        Terminal::NUM,
        Terminal::STRLITERAL,
        Terminal::CHARLITERAL,
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
        .init_state = init_state,
        .valid_states = valid_states,
        .accepting = accepting,
        .transition = transition_func,
    };
}

static const std::map<std::string, Terminal> keywords = {
    {"mod", Terminal::MODULE},
    {"import", Terminal::IMPORT},
    {"fn", Terminal::FN},
    {"let", Terminal::LET},
    {"if", Terminal::IF},
    {"while", Terminal::WHILE},
    {"else", Terminal::ELSE},
    {"return", Terminal::RET},
    {"as", Terminal::AS},
    {"i32", Terminal::I32},
    {"bool", Terminal::BOOL},
    {"char", Terminal::CHAR},
    {"none", Terminal::CHAR},
    {"true", Terminal::TRUE},
    {"false", Terminal::FALSE},
    {"new", Terminal::NEW},
    {"delete", Terminal::DELETE},
};

std::vector<Token> scan(std::string_view input) {
    DFA dfa = make_nex_lang_dfa();

    std::vector<Token> tokens = maximal_munch_scan(input, dfa);
    for (auto& token : tokens) {
        if (keywords.count(token.lexeme)) {
            token.kind = keywords.at(token.lexeme);
        } else if (token.kind == Terminal::ZERO) {
            token.kind = Terminal::NUM;
        }
    }

    static const std::set<Terminal> sep_set1 = {
        Terminal::FN,
        Terminal::LET,
        Terminal::IF,
        Terminal::WHILE,
        Terminal::ELSE,
        Terminal::I32,
        Terminal::BOOL,
        Terminal::CHAR,
        Terminal::NONE,
        Terminal::ID,
        Terminal::NUM,
        Terminal::STRLITERAL,
        Terminal::CHARLITERAL,
        Terminal::TRUE,
        Terminal::FALSE};
    static const std::set<Terminal> sep_set2 = {
        Terminal::EQ,
        Terminal::NE,
        Terminal::LT,
        Terminal::LE,
        Terminal::GT,
        Terminal::GE,
        Terminal::OR,
        Terminal::AND,
        Terminal::ASSIGN,
        Terminal::ARROW};

    bool prev_set1 = false;
    bool prev_set2 = false;

    std::vector<Token> result;
    for (const auto& token : tokens) {
        if (sep_set1.contains(token.kind)) {
            if (prev_set1) {
                std::cerr << "Invalid consecutive keywords!" << std::endl;
                exit(1);
            }
            prev_set1 = true;
        } else {
            prev_set1 = false;
        }

        if (sep_set2.contains(token.kind)) {
            if (prev_set2) {
                std::cerr << "Invalid consecutive symbols!" << std::endl;
                exit(1);
            }
            prev_set2 = true;
        } else {
            prev_set2 = false;
        }

        const std::set<Terminal> remove_symbols = {
            Terminal::SPACE,
            Terminal::NEWLINE,
            Terminal::CARRIAGERETURN,
            Terminal::TAB,
            Terminal::COMMENT};
        if (!remove_symbols.contains(token.kind)) {
            result.push_back(token);
        }
    }

    result.insert(result.begin(), Token {Terminal::BOFS, ""});
    result.push_back(Token {Terminal::EOFS, ""});

    return result;
}
