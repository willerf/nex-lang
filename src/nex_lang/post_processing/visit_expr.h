
#pragma once

#include <memory>
#include <vector>

#include "ast_node.h"
#include "module_table.h"
#include "program_context.h"
#include "symbol_table.h"
#include "typed_expr.h"

struct ASTNode;
struct Code;
struct ProgramContext;

TypedExpr visit_expr(
    ASTNode root,
    bool read_address,
    SymbolTable& symbol_table,
    ProgramContext& program_context,
    std::vector<std::shared_ptr<Code>>& static_data
);
