
#pragma once

#include <memory>
#include <vector>

#include "ast_node.h"
#include "code.h"
#include "symbol_table.h"
#include "typed_procedure.h"

struct ASTNode;

std::shared_ptr<Code> visit_stmt(
    ASTNode root,
    std::shared_ptr<TypedProcedure> curr_proc,
    SymbolTable& symbol_table,
    std::vector<std::shared_ptr<Code>>& static_data
);

std::shared_ptr<Code> visit_stmts(
    ASTNode root,
    std::shared_ptr<TypedProcedure> curr_proc,
    SymbolTable& symbol_table,
    std::vector<std::shared_ptr<Code>>& static_data
);
