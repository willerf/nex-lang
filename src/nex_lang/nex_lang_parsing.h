
#include <span>
#include <string_view>
#include <vector>

#include "ast_node.h"
#include "dfa.h"
#include "grammar.h"
#include "token.h"

Grammar make_nex_lang_grammar();
ASTNode parse(std::span<Token> input);
