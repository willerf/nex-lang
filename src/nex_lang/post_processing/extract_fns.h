
#pragma once

#include "ast_node.h"
#include "program_context.h"
#include "symbol_table.h"

struct ASTNode;
struct ProgramContext;

void extract_fns(
    ASTNode root,
    SymbolTable& symbol_table,
    ProgramContext& program_context
);
void extract_fn(
    ASTNode root,
    SymbolTable& symbol_table,
    ProgramContext& program_context
);
