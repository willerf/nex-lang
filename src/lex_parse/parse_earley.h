

#pragma once

#include <optional>
#include <span>

#include "ast_node.h"
#include "grammar.h"
#include "token.h"

std::optional<ASTNode> parse_earley(std::span<Token> input, Grammar& grammar);
