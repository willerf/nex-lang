
#pragma once

#include <memory>
#include <vector>

#include "ast_node.h"
#include "symbol_table.h"
#include "typed_procedure.h"

struct ASTNode;
struct Code;

std::vector<std::shared_ptr<TypedProcedure>> visit_s(
    ASTNode root,
    SymbolTable& symbol_table,
    std::vector<std::shared_ptr<Code>>& static_data
);
