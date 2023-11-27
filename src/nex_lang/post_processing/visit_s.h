
#pragma once

#include <memory>
#include <vector>

#include "ast_node.h"
#include "module_table.h"
#include "program_context.h"
#include "typed_procedure.h"

struct ASTNode;
struct Code;
struct ProgramContext;

std::vector<std::shared_ptr<TypedProcedure>> visit_s(
    ASTNode root,
    ProgramContext& program_context,
    std::vector<std::shared_ptr<Code>>& static_data
);
