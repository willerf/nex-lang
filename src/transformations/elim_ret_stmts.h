
#pragma once

#include <memory>

#include "code.h"
#include "label.h"
#include "ret_stmt.h"
#include "visitor.h"

class ElimRetStmts: public Visitor<std::shared_ptr<Code>> {
    std::shared_ptr<Label> proc_end;

  public:
    ElimRetStmts(std::shared_ptr<Label> proc_end);
    std::shared_ptr<Code> visit(std::shared_ptr<RetStmt>) override;
};
