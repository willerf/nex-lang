
#pragma once

#include "dfa.h"
#include "token.h"

#include <string_view>

Token scan_one(std::string_view input, DFA& dfa);
std::vector<Token> scan(std::string_view input, DFA& dfa);

