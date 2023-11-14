
#pragma once

#include "ast_node.h"
#include "module_table.h"
#include "symbol_table.h"

void visit_imports(
    ASTNode root,
    SymbolTable& symbol_table,
    ModuleTable& module_table
);
void visit_import(
    ASTNode root,
    SymbolTable& symbol_table,
    ModuleTable& module_table
);
