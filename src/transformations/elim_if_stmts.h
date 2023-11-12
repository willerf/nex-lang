
#pragma once

#include <memory>

#include "code.h"
#include "if_stmt.h"
#include "visitor.h"

class ElimIfStmts: public Visitor<std::shared_ptr<Code>> {
  public:
    std::shared_ptr<Code> visit(std::shared_ptr<Code>) override;
    std::shared_ptr<Code> visit(std::shared_ptr<IfStmt>) override;
};
