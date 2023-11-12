
#pragma once

#include <memory>
#include <vector>

#include "ast_node.h"
#include "symbol_table.h"
#include "typed_variable.h"

struct ASTNode;

std::vector<std::shared_ptr<TypedVariable>>
visit_params(ASTNode root, SymbolTable& symbol_table);

std::vector<std::shared_ptr<TypedVariable>>
visit_optparams(ASTNode root, SymbolTable& symbol_table);
