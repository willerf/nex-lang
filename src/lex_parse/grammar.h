
#pragma once

#include <map>
#include <set>

#include "state.h"

struct Production {
    NonTerminal lhs;
    std::vector<State> rhs;
    bool operator==(const Production&) const = default;
};

struct Grammar {
    NonTerminal start;
    std::map<NonTerminal, std::vector<Production>> productions;
};
