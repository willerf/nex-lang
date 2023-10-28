
#include <string_view>

#include "dfa.h"
#include "token.h"

DFA make_dfa();
std::vector<Token> scan(std::string_view input);
