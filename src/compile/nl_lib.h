
#pragma once

#include <string>
#include <utility>
#include <vector>

#include "ast_node.h"
#include "module_table.h"
#include "program_context.h"

struct ASTNode;

void nl_lib_import(
    std::string import_name,
    std::vector<std::string>& import_list,
    ProgramContext& program_context,
    std::vector<std::pair<std::string, ASTNode>>& modules
);
