
#pragma once

#include <optional>
#include <span>

#include "ast_node.h"
#include "grammar.h"
#include "memo_map.h"
#include "token.h"

struct Grammar;
struct Token;

std::optional<ASTNode>
parse_cyk(std::span<Token> input, const Grammar& grammar);
