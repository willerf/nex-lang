
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

std::vector<TypedExpr> visit_args(
    ASTNode root,
    SymbolTable& symbol_table,
    ProgramContext& program_context,
    std::vector<std::shared_ptr<Code>>& static_data
);

std::vector<TypedExpr> visit_optargs(
    ASTNode root,
    SymbolTable& symbol_table,
    ProgramContext& program_context,
    std::vector<std::shared_ptr<Code>>& static_data
);
