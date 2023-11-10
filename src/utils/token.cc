
#include "token.h"

bool Token::operator==(const Token& other) const {
    return kind == other.kind && lexeme == other.lexeme;
}

std::ostream& operator<<(std::ostream& out, const Token& token) {
    out << "(" << state::to_string(token.kind) << ", " << token.lexeme << ")";
    return out;
}
