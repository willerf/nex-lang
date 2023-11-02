
#include "token.h"

std::ostream& operator<<(std::ostream& out, const Token& token) {
    out << "(" << state::to_string(token.kind) << ", " << token.lexeme << ")";
    return out;
}
