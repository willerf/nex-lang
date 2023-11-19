
#pragma once

#include "ast_node.h"
#include "module_table.h"

void nl_lib_import(
    std::string import_name,
    std::vector<std::string>& import_list,
    ModuleTable& module_table,
    std::vector<std::pair<std::string, ASTNode>>& modules
);
