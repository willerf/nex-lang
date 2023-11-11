
#pragma once

#include "ast_node.h"
#include "symbol_table.h"
#include "typed_expr.h"
std::vector<TypedExpr> visit_args(
    ASTNode root,
    SymbolTable& symbol_table,
    std::vector<std::shared_ptr<Code>>& static_data
);

std::vector<TypedExpr> visit_optargs(
    ASTNode root,
    SymbolTable& symbol_table,
    std::vector<std::shared_ptr<Code>>& static_data
);
