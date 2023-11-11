
#pragma once

#include "ast_node.h"
#include "symbol_table.h"
#include "typed_variable.h"

std::shared_ptr<TypedVariable>
visit_vardef(ASTNode root, SymbolTable& symbol_table);
