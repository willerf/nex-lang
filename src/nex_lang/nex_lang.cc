#include "nex_lang.h"

#include <stdlib.h>

#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <variant>

#include "scanning.h"
#include "state.h"

static std::map<char, Terminal> one_char_symbols = {
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

static std::map<std::string, Terminal> two_char_symbols = {
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
        if (one_char_symbols.count(c)) {
            return one_char_symbols[c];
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

DFA make_dfa() {
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

static std::map<std::string, Terminal> keywords = {
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
    DFA dfa = make_dfa();

    std::vector<Token> tokens = maximal_munch_scan(input, dfa);
    for (auto& token : tokens) {
        if (keywords.count(token.lexeme)) {
            token.kind = keywords[token.lexeme];
        } else if (token.kind == Terminal::ZERO) {
            token.kind = Terminal::NUM;
        }
    }

    std::set<Terminal> sep_set1 = {
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
    std::set<Terminal> sep_set2 = {
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
        if (sep_set1.count(token.kind)) {
            if (prev_set1) {
                std::cerr << "Invalid consecutive keywords!" << std::endl;
                exit(1);
            }
            prev_set1 = true;
        } else {
            prev_set1 = false;
        }

        if (sep_set2.count(token.kind)) {
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
        if (!remove_symbols.count(token.kind)) {
            result.push_back(token);
        }
    }

    result.insert(result.begin(), Token {Terminal::BOFS, ""});
    result.push_back(Token {Terminal::EOFS, ""});

    return result;
}

static std::map<NonTerminal, std::vector<Production>> productions = {
    {NonTerminal::s,
     {{NonTerminal::s,
       {Terminal::BOFS,
        NonTerminal::module,
        NonTerminal::imports,
        NonTerminal::fns,
        Terminal::EOFS}}}},
    {NonTerminal::module,
     {{NonTerminal::module, {Terminal::MODULE, Terminal::ID, Terminal::SEMI}}}},
    {NonTerminal::imports,
     {{NonTerminal::imports, {NonTerminal::import, NonTerminal::imports}},
      {NonTerminal::imports, {}}}},
    {NonTerminal::import,
     {{NonTerminal::import, {Terminal::IMPORT, Terminal::ID, Terminal::SEMI}}}},
    {NonTerminal::fns,
     {{NonTerminal::fns, {NonTerminal::fn, NonTerminal::fns}},
      {NonTerminal::fns, {NonTerminal::fn}}}},
    {NonTerminal::fn,
     {{NonTerminal::fn,
       {Terminal::FN,
        Terminal::ID,
        Terminal::LPAREN,
        NonTerminal::optparams,
        Terminal::RPAREN,
        Terminal::ARROW,
        NonTerminal::type,
        NonTerminal::stmtblock}},
      {NonTerminal::fn,
       {Terminal::FN,
        Terminal::ID,
        Terminal::LPAREN,
        NonTerminal::optparams,
        Terminal::RPAREN,
        NonTerminal::stmtblock}}}},
    {NonTerminal::optparams,
     {{NonTerminal::optparams, {NonTerminal::params}},
      {NonTerminal::optparams, {}}}},
    {NonTerminal::params,
     {{NonTerminal::params,
       {NonTerminal::vardef, Terminal::COMMA, NonTerminal::params}},
      {NonTerminal::params, {NonTerminal::vardef}}}},
    {NonTerminal::vardef,
     {{NonTerminal::vardef,
       {Terminal::ID, Terminal::COLON, NonTerminal::type}}}},
    {NonTerminal::type,
     {{NonTerminal::type, {Terminal::I32}},
      {NonTerminal::type, {Terminal::BOOL}},
      {NonTerminal::type, {Terminal::CHAR}},
      {NonTerminal::type, {Terminal::NONE}},
      {NonTerminal::type, {Terminal::STAR, NonTerminal::type}},
      {NonTerminal::type,
       {Terminal::LPAREN, NonTerminal::type, Terminal::RPAREN}}}},
    {NonTerminal::stmtblock,
     {{NonTerminal::stmtblock,
       {Terminal::LBRACE, NonTerminal::stmts, Terminal::RBRACE}}}},
    {NonTerminal::stmts,
     {{NonTerminal::stmts, {NonTerminal::stmt, NonTerminal::stmts}},
      {NonTerminal::stmts, {NonTerminal::stmt}}}},
    {NonTerminal::stmt,
     {{NonTerminal::stmt,
       {Terminal::LET,
        NonTerminal::vardef,
        Terminal::ASSIGN,
        NonTerminal::expr,
        Terminal::SEMI}},
      {NonTerminal::stmt,
       {NonTerminal::expr,
        Terminal::ASSIGN,
        NonTerminal::expr,
        Terminal::SEMI}},
      {NonTerminal::stmt, {NonTerminal::expr, Terminal::SEMI}},
      {NonTerminal::stmt,
       {Terminal::IF,
        Terminal::LPAREN,
        NonTerminal::expr,
        Terminal::RPAREN,
        NonTerminal::stmtblock,
        Terminal::ELSE,
        NonTerminal::stmtblock}},
      {NonTerminal::stmt,
       {Terminal::IF,
        Terminal::LPAREN,
        NonTerminal::expr,
        Terminal::RPAREN,
        NonTerminal::stmtblock}},
      {NonTerminal::stmt,
       {Terminal::WHILE,
        Terminal::LPAREN,
        NonTerminal::expr,
        Terminal::RPAREN,
        NonTerminal::stmtblock}},
      {NonTerminal::stmt, {Terminal::RET, NonTerminal::expr, Terminal::SEMI}},
      {NonTerminal::stmt,
       {Terminal::DELETE, NonTerminal::expr, Terminal::SEMI}}}},
    {NonTerminal::expr, {{NonTerminal::expr, {NonTerminal::exprp1}}}},
    {NonTerminal::exprp1,
     {{NonTerminal::exprp1, {NonTerminal::exprp2}},
      {NonTerminal::exprp1,
       {NonTerminal::exprp1, Terminal::OR, NonTerminal::exprp2}}}},
    {NonTerminal::exprp2,
     {{NonTerminal::exprp2, {NonTerminal::exprp3}},
      {NonTerminal::exprp2,
       {NonTerminal::exprp2, Terminal::AND, NonTerminal::exprp3}}}},
    {NonTerminal::exprp3,
     {{NonTerminal::exprp3, {NonTerminal::exprp4}},
      {NonTerminal::exprp3,
       {NonTerminal::exprp3, Terminal::EQ, NonTerminal::exprp4}},
      {NonTerminal::exprp3,
       {NonTerminal::exprp3, Terminal::NE, NonTerminal::exprp4}}}},
    {NonTerminal::exprp4,
     {{NonTerminal::exprp4, {NonTerminal::exprp5}},
      {NonTerminal::exprp4,
       {NonTerminal::exprp4, Terminal::LT, NonTerminal::exprp5}},
      {NonTerminal::exprp4,
       {NonTerminal::exprp4, Terminal::GT, NonTerminal::exprp5}},
      {NonTerminal::exprp4,
       {NonTerminal::exprp4, Terminal::LE, NonTerminal::exprp5}},
      {NonTerminal::exprp4,
       {NonTerminal::exprp4, Terminal::GE, NonTerminal::exprp5}}}},
    {NonTerminal::exprp5,
     {{NonTerminal::exprp5, {NonTerminal::exprp6}},
      {NonTerminal::exprp5,
       {NonTerminal::exprp5, Terminal::PLUS, NonTerminal::exprp6}},
      {NonTerminal::exprp5,
       {NonTerminal::exprp5, Terminal::MINUS, NonTerminal::exprp6}}}},
    {NonTerminal::exprp6,
     {{NonTerminal::exprp6, {NonTerminal::exprp7}},
      {NonTerminal::exprp6,
       {NonTerminal::exprp6, Terminal::STAR, NonTerminal::exprp7}},
      {NonTerminal::exprp6,
       {NonTerminal::exprp6, Terminal::SLASH, NonTerminal::exprp7}},
      {NonTerminal::exprp6,
       {NonTerminal::exprp6, Terminal::PCT, NonTerminal::exprp7}}}},
    {NonTerminal::exprp7,
     {{NonTerminal::exprp7, {NonTerminal::exprp8}},
      {NonTerminal::exprp7, {Terminal::NOT, NonTerminal::exprp8}},
      {NonTerminal::exprp7, {Terminal::STAR, NonTerminal::exprp8}}}},
    {NonTerminal::exprp8,
     {{NonTerminal::exprp8, {NonTerminal::exprp9}},
      {NonTerminal::exprp8,
       {NonTerminal::exprp8, Terminal::AS, NonTerminal::type}}}},
    {NonTerminal::exprp9,
     {
         {NonTerminal::exprp9, {Terminal::ID}},
         {NonTerminal::exprp9, {Terminal::NUM}},
         {NonTerminal::exprp9, {Terminal::AMPERSAND, Terminal::ID}},
         {NonTerminal::exprp9, {Terminal::STRLITERAL}},
         {NonTerminal::exprp9, {Terminal::CHARLITERAL}},
         {NonTerminal::exprp9,
          {Terminal::LPAREN, NonTerminal::expr, Terminal::RPAREN}},
         {NonTerminal::exprp9,
          {Terminal::ID,
           Terminal::LPAREN,
           NonTerminal::optargs,
           Terminal::RPAREN}},
         {NonTerminal::exprp9, {Terminal::NEW, NonTerminal::typeinit}},
         {NonTerminal::exprp9,
          {Terminal::ID,
           Terminal::LBRACKET,
           NonTerminal::expr,
           Terminal::RBRACKET}},
     }},
    {NonTerminal::optargs,
     {{NonTerminal::optargs, {NonTerminal::args}}, {NonTerminal::optargs, {}}}},
    {NonTerminal::args,
     {{NonTerminal::args,
       {NonTerminal::expr, Terminal::COMMA, NonTerminal::args}},
      {NonTerminal::args, {NonTerminal::expr}}}},
    {NonTerminal::typeinit,
     {
         {NonTerminal::typeinit, {NonTerminal::type}},
         {NonTerminal::typeinit,
          {NonTerminal::type,
           Terminal::LBRACKET,
           NonTerminal::expr,
           Terminal::RBRACKET}},
     }}};

static std::set<Terminal> terminals = {
    Terminal::BOFS,        Terminal::EOFS,     Terminal::FN,
    Terminal::ID,          Terminal::LPAREN,   Terminal::RPAREN,
    Terminal::ARROW,       Terminal::LBRACE,   Terminal::RBRACE,
    Terminal::COMMA,       Terminal::COLON,    Terminal::I32,
    Terminal::LET,         Terminal::ASSIGN,   Terminal::SEMI,
    Terminal::IF,          Terminal::ELSE,     Terminal::RET,
    Terminal::OR,          Terminal::AND,      Terminal::EQ,
    Terminal::NE,          Terminal::LT,       Terminal::GT,
    Terminal::LE,          Terminal::GE,       Terminal::PLUS,
    Terminal::MINUS,       Terminal::STAR,     Terminal::SLASH,
    Terminal::PCT,         Terminal::NOT,      Terminal::NUM,
    Terminal::AMPERSAND,   Terminal::WHILE,    Terminal::BOOL,
    Terminal::TRUE,        Terminal::FALSE,    Terminal::STRLITERAL,
    Terminal::CHARLITERAL, Terminal::CHAR,     Terminal::AS,
    Terminal::MODULE,      Terminal::IMPORT,   Terminal::NONE,
    Terminal::LBRACKET,    Terminal::RBRACKET, Terminal::NEW,
    Terminal::DELETE,
};

static std::set<NonTerminal> non_terminals = {
    NonTerminal::s,         NonTerminal::fns,      NonTerminal::fn,
    NonTerminal::optparams, NonTerminal::params,   NonTerminal::vardef,
    NonTerminal::type,      NonTerminal::stmts,    NonTerminal::stmt,
    NonTerminal::expr,      NonTerminal::exprp1,   NonTerminal::exprp2,
    NonTerminal::exprp3,    NonTerminal::exprp4,   NonTerminal::exprp5,
    NonTerminal::exprp6,    NonTerminal::exprp7,   NonTerminal::exprp8,
    NonTerminal::exprp9,    NonTerminal::optargs,  NonTerminal::args,
    NonTerminal::stmtblock, NonTerminal::module,   NonTerminal::imports,
    NonTerminal::import,    NonTerminal::typeinit,
};

Grammar make_grammar() {
    return Grammar {
        .non_terminals = non_terminals,
        .terminals = terminals,
        .start = NonTerminal::s,
        .productions = productions,
    };
}
