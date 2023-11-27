
#include "state.h"

#include <string>

std::string state::to_string(Terminal terminal) {
    switch (terminal) {
        case Terminal::BOFS:
            return "BOFS";
        case Terminal::EOFS:
            return "EOFS";
        case Terminal::MODULE:
            return "MODULE";
        case Terminal::IMPORT:
            return "IMPORT";
        case Terminal::FN:
            return "FN";
        case Terminal::ID:
            return "ID";
        case Terminal::LPAREN:
            return "LPAREN";
        case Terminal::RPAREN:
            return "RPAREN";
        case Terminal::ARROW:
            return "ARROW";
        case Terminal::LBRACE:
            return "LBRACE";
        case Terminal::RBRACE:
            return "RBRACE";
        case Terminal::LBRACKET:
            return "LBRACKET";
        case Terminal::RBRACKET:
            return "RBRACKET";
        case Terminal::COMMA:
            return "COMMA";
        case Terminal::COLON:
            return "COLON";
        case Terminal::AS:
            return "AS";
        case Terminal::NEW:
            return "NEW";
        case Terminal::DELETE:
            return "DELETE";
        case Terminal::I32:
            return "I32";
        case Terminal::BOOL:
            return "BOOL";
        case Terminal::CHAR:
            return "CHAR";
        case Terminal::NONE:
            return "NONE";
        case Terminal::TRUE:
            return "TRUE";
        case Terminal::FALSE:
            return "FALSE";
        case Terminal::LET:
            return "LET";
        case Terminal::ASSIGN:
            return "ASSIGN";
        case Terminal::SEMI:
            return "SEMI";
        case Terminal::IF:
            return "IF";
        case Terminal::WHILE:
            return "WHILE";
        case Terminal::ELSE:
            return "ELSE";
        case Terminal::RET:
            return "RET";
        case Terminal::OR:
            return "OR";
        case Terminal::AND:
            return "AND";
        case Terminal::EQ:
            return "EQ";
        case Terminal::NE:
            return "NE";
        case Terminal::LT:
            return "LT";
        case Terminal::GT:
            return "GT";
        case Terminal::LE:
            return "LE";
        case Terminal::GE:
            return "GE";
        case Terminal::PLUS:
            return "PLUS";
        case Terminal::MINUS:
            return "MINUS";
        case Terminal::STAR:
            return "STAR";
        case Terminal::SLASH:
            return "SLASH";
        case Terminal::AMPERSAND:
            return "AMPERSAND";
        case Terminal::PIPE:
            return "PIPE";
        case Terminal::PCT:
            return "PCT";
        case Terminal::NOT:
            return "NOT";
        case Terminal::NUM:
            return "NUM";
        case Terminal::STRLITERALNF:
            return "STRLITERALNF";
        case Terminal::CHARLITERALNF:
            return "CHARLITERALNF";
        case Terminal::STRLITERAL:
            return "STRLITERAL";
        case Terminal::CHARLITERAL:
            return "CHARLITERAL";
        case Terminal::ZERO:
            return "ZERO";
        case Terminal::SPACE:
            return "SPACE";
        case Terminal::CARRIAGERETURN:
            return "CARRIAGERETURN";
        case Terminal::NEWLINE:
            return "NEWLINE";
        case Terminal::TAB:
            return "TAB";
        case Terminal::COMMENT:
            return "COMMENT";
        case Terminal::START:
            return "START";
        default:
            return "UNKNOWN";
    }
}

std::string state::to_string(NonTerminal non_terminal) {
    switch (non_terminal) {
        case NonTerminal::s:
            return "s";
        case NonTerminal::module:
            return "module";
        case NonTerminal::imports:
            return "imports";
        case NonTerminal::import:
            return "import";
        case NonTerminal::fns:
            return "fns";
        case NonTerminal::fn:
            return "fn";
        case NonTerminal::optparams:
            return "optparams";
        case NonTerminal::params:
            return "params";
        case NonTerminal::vardef:
            return "vardef";
        case NonTerminal::type:
            return "type";
        case NonTerminal::stmtblock:
            return "stmtblock";
        case NonTerminal::stmts:
            return "stmts";
        case NonTerminal::stmt:
            return "stmt";
        case NonTerminal::expr:
            return "expr";
        case NonTerminal::exprp1:
            return "exprp1";
        case NonTerminal::exprp2:
            return "exprp2";
        case NonTerminal::exprp3:
            return "exprp3";
        case NonTerminal::exprp4:
            return "exprp4";
        case NonTerminal::exprp5:
            return "exprp5";
        case NonTerminal::exprp6:
            return "exprp6";
        case NonTerminal::exprp7:
            return "exprp7";
        case NonTerminal::exprp8:
            return "exprp8";
        case NonTerminal::exprp9:
            return "exprp9";
        case NonTerminal::typeinit:
            return "typeinit";
        case NonTerminal::optargs:
            return "optargs";
        case NonTerminal::args:
            return "args";
        default:
            return "UNKNOWN";
    }
}

std::string state::to_string(State state) {
    if (std::holds_alternative<Terminal>(state)) {
        return to_string(std::get<Terminal>(state));
    }
    if (std::holds_alternative<NonTerminal>(state)) {
        return to_string(std::get<NonTerminal>(state));
    }
    return "UNKNOWN";
}
