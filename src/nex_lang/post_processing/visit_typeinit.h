
#pragma once

#include "symbol_table.h"
#include "typed_expr.h"
#include "module_table.h"

struct ASTNode;
struct Code;

TypedExpr visit_typeinit(
    ASTNode root,
    bool read_address,
    SymbolTable& symbol_table,
    ModuleTable& module_table,
    std::vector<std::shared_ptr<Code>>& static_data
);

