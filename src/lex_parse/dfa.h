
#pragma once

#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>

#include "state.h"

struct DFA {
    std::set<char> alphabet;
    Terminal init_state;
    std::set<Terminal> valid_states;
    std::set<Terminal> accepting;
    std::function<std::optional<Terminal>(Terminal, char)> transition;
};
