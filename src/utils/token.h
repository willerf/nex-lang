
#pragma once

#include <ostream>

#include "dfa.h"

struct Token {
    Terminal kind;
    std::string lexeme;
    size_t line_no = 0;

    bool operator==(const Token&) const;
};

std::ostream& operator<<(std::ostream&, const Token&);
