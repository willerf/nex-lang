
#pragma once

#include <optional>
#include <span>

#include "ast_node.h"
#include "grammar.h"
#include "token.h"

std::optional<ASTNode> parse_cyk(std::span<Token> input, Grammar& grammar);
