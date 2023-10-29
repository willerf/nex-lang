
#pragma once

#include "ast_node.h"
#include "procedure.h"

std::vector<std::shared_ptr<Procedure>> generate(ASTNode root);
