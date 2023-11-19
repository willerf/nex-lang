
#pragma once

#include "ast_node.h"
#include "module_table.h"
#include "symbol_table.h"

struct ASTNode;

std::vector<std::string>
extract_imports(ASTNode root, ModuleTable& module_table);

std::vector<std::string>
extract_import(ASTNode root, ModuleTable& module_table);
