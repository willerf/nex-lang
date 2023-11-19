
#pragma once

#include <string>
#include <vector>

#include "ast_node.h"
#include "module_table.h"

struct ASTNode;

std::vector<std::string> extract_s(ASTNode root, ModuleTable& module_table);
