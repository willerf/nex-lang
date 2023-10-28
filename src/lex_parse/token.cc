
#include "token.h"

std::ostream& operator<<(std::ostream& out, const Token& token) {
    out << "(" << token.kind << ", " << token.lexeme << ")";
    return out;
}
