
#pragma once

#include <map>

#include "ast_node.h"
#include "typed_procedure.h"

std::vector<std::shared_ptr<TypedProcedure>>
generate(ASTNode root, std::vector<std::shared_ptr<Code>>& static_data);
