
#pragma once

#include "ast_node.h"
#include "module_table.h"

struct ASTNode;

std::vector<std::string>
extract_symbols(ASTNode root, ModuleTable& module_table);
