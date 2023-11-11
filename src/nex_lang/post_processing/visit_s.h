
#pragma once

#include "ast_node.h"
#include "symbol_table.h"
#include "typed_procedure.h"

std::vector<std::shared_ptr<TypedProcedure>> visit_s(
    ASTNode root,
    SymbolTable& symbol_table,
    std::vector<std::shared_ptr<Code>>& static_data
);
