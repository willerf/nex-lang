
#pragma once

#include <map>
#include <set>

#include "state.h"

struct Production {
    State lhs;
    std::vector<State> rhs;
};

struct Grammar {
    std::set<State> non_terminals;
    std::set<State> terminals;
    State start;
    std::map<State, std::vector<Production>> productions;
};
