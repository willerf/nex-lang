
#pragma once

#include <memory>
#include <vector>

#include "ast_node.h"
#include "module_table.h"
#include "program_context.h"
#include "symbol_table.h"
#include "typed_procedure.h"

struct ASTNode;
struct Code;

std::shared_ptr<TypedProcedure> visit_fn(
    ASTNode root,
    SymbolTable& symbol_table,
    ProgramContext& program_context,
    std::vector<std::shared_ptr<Code>>& static_data
);

std::vector<std::shared_ptr<TypedProcedure>> visit_fns(
    ASTNode root,
    SymbolTable& symbol_table,
    ProgramContext& program_context,
    std::vector<std::shared_ptr<Code>>& static_data
);
