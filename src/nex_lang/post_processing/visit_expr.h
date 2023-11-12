
#pragma once

#include <memory>
#include <vector>

#include "ast_node.h"
#include "module_table.h"
#include "symbol_table.h"
#include "typed_expr.h"

struct ASTNode;
struct Code;

TypedExpr visit_expr(
    ASTNode root,
    bool read_address,
    SymbolTable& symbol_table,
    ModuleTable& module_table,
    std::vector<std::shared_ptr<Code>>& static_data
);
