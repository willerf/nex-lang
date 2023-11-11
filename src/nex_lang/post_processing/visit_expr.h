
#pragma once

#include "ast_node.h"
#include "symbol_table.h"
#include "typed_expr.h"
TypedExpr visit_expr(
    ASTNode root,
    bool read_address,
    SymbolTable& symbol_table,
    std::vector<std::shared_ptr<Code>>& static_data
);
