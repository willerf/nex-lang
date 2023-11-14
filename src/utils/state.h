
#pragma once
#include <string>
#include <variant>

enum class Terminal {
    BOFS,
    EOFS,
    MODULE,
    IMPORT,
    FN,
    ID,
    LPAREN,
    RPAREN,
    ARROW,
    LBRACE,
    RBRACE,
    COMMA,
    COLON,
    AS,
    I32,
    BOOL,
    CHAR,
    TRUE,
    FALSE,
    LET,
    ASSIGN,
    SEMI,
    IF,
    WHILE,
    ELSE,
    RET,
    OR,
    AND,
    EQ,
    NE,
    LT,
    GT,
    LE,
    GE,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    AMPERSAND,
    PIPE,
    PCT,
    NOT,
    NUM,
    STRLITERALNF,
    CHARLITERALNF,
    STRLITERAL,
    CHARLITERAL,
    ZERO,
    SPACE,
    CARRIAGERETURN,
    NEWLINE,
    TAB,
    COMMENT,
    START
};

enum class NonTerminal {
    s,
    module,
    imports,
    import,
    fns,
    fn,
    optparams,
    params,
    vardef,
    type,
    stmtblock,
    stmts,
    stmt,
    expr,
    exprp1,
    exprp2,
    exprp3,
    exprp4,
    exprp5,
    exprp6,
    exprp7,
    exprp8,
    exprp9,
    optargs,
    args
};

using State = std::variant<Terminal, NonTerminal>;

namespace state {
std::string to_string(Terminal terminal);
std::string to_string(NonTerminal non_terminal);
std::string to_string(State state);
}  // namespace state
