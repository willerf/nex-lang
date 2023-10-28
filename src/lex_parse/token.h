
#pragma once

#include <ostream>

#include "dfa.h"

struct Token {
    State kind;
    std::string lexeme;

    bool operator==(const Token&) const = default;
};

std::ostream& operator<<(std::ostream&, const Token&);
