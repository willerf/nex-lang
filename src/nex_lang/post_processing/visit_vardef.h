
#pragma once

#include <memory>

#include "ast_node.h"
#include "program_context.h"
#include "symbol_table.h"
#include "typed_variable.h"

struct ASTNode;
struct ProgramContext;

std::shared_ptr<TypedVariable> visit_vardef(
    ASTNode root,
    SymbolTable& symbol_table,
    ProgramContext& program_context
);
