#pragma once

#include <memory>
#include <optional>

#include "ast_node.h"
#include "code.h"
#include "code_visit.h"
#include "nl_type.h"

struct Expr: CodeVisit<Expr> {
    std::shared_ptr<ASTNode> code;
    std::shared_ptr<NLType> nl_type = nullptr;
    Expr(std::shared_ptr<ASTNode> code);
};
