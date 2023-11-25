
#pragma once

#include "ast_node.h"
#include "module_table.h"
#include "program_context.h"
#include "symbol_table.h"

struct ASTNode;

void visit_imports(
    ASTNode root,
    SymbolTable& symbol_table,
    ProgramContext& program_context
);
void visit_import(
    ASTNode root,
    SymbolTable& symbol_table,
    ProgramContext& program_context
);
