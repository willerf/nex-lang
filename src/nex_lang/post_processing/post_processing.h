
#pragma once

#include <map>
#include <memory>
#include <vector>

#include "ast_node.h"
#include "module_table.h"
#include "typed_procedure.h"

struct ASTNode;
struct Code;

std::vector<std::shared_ptr<TypedProcedure>> generate(
    ASTNode root,
    std::vector<std::shared_ptr<Code>>& static_data,
    ModuleTable& module_table
);
