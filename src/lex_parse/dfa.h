
#pragma once

#include <set>
#include <map>
#include <functional>
#include <optional>
#include <string>

typedef std::string State;

struct DFA {
    std::set<char> alphabet;
    State init_state;
    std::set<State> valid_states;
    std::set<State> accepting;
    std::function<std::optional<State>(State, char)> transition;
};

