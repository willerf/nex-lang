
#pragma once

#include <stddef.h>

#include <ostream>
#include <string>

#include "dfa.h"

enum class Terminal;

struct Token {
    Terminal kind;
    std::string lexeme;
    size_t line_no = 0;

    bool operator==(const Token&) const;
};

std::ostream& operator<<(std::ostream&, const Token&);
