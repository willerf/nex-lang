
#pragma once

#include "label.h"
#include "visitor.h"

class ElimRetStmts: public Visitor<std::shared_ptr<Code>> {
    std::shared_ptr<Label> proc_end;

  public:
    ElimRetStmts(std::shared_ptr<Label> proc_end);
    std::shared_ptr<Code> visit(std::shared_ptr<Code>) override;
    std::shared_ptr<Code> visit(std::shared_ptr<RetStmt>) override;
};
