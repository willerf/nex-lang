
#pragma once

#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>

#include "state.h"

struct DFA {
    std::set<char> alphabet;
    State init_state;
    std::set<State> valid_states;
    std::set<State> accepting;
    std::function<std::optional<State>(State, char)> transition;
};
