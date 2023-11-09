
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
    std::set<NonTerminal> non_terminals;
    std::set<Terminal> terminals;
    NonTerminal start;
    std::map<NonTerminal, std::vector<Production>> productions;
};
