
#pragma once

#include <memory>

#include "ast_node.h"
#include "nl_type.h"
#include "program_context.h"

struct ASTNode;

std::shared_ptr<NLType>
visit_type(ASTNode root, ProgramContext& program_context);
