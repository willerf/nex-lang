
#pragma once

#include <string_view>

#include "dfa.h"
#include "token.h"

Token scan_one(std::string_view input, DFA& dfa);
std::vector<Token> scan(std::string_view input, DFA& dfa);
