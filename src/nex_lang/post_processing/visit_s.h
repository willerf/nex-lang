
#pragma once

#include <memory>
#include <vector>

#include "ast_node.h"
#include "typed_procedure.h"

struct ASTNode;
struct Code;
struct ProgramContext;

std::vector<std::shared_ptr<TypedProcedure>> visit_s(
    ASTNode root
);
