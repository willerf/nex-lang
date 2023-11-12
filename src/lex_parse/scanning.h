
#pragma once

#include <string_view>
#include <vector>

#include "dfa.h"
#include "token.h"

struct DFA;

Token scan_one(std::string_view input, DFA& dfa);
std::vector<Token> maximal_munch_scan(std::string_view input, DFA& dfa);
