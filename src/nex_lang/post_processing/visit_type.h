
#pragma once

#include <memory>

#include "ast_node.h"
#include "nl_type.h"

struct ASTNode;

std::shared_ptr<NLType> visit_type(ASTNode root);
