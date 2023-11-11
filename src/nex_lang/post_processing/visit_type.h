
#pragma once

#include "ast_node.h"
#include "nl_type.h"

std::shared_ptr<NLType> visit_type(ASTNode root);
