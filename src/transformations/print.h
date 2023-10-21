
#pragma once

#include <vector>

#include "visitor.h"

class Print: public Visitor<void> {
    int depth = 0;

  public:
    void visit(std::shared_ptr<Code>) override;
    void visit(std::shared_ptr<Block>) override;
    void visit(std::shared_ptr<Word>) override;
    void visit(std::shared_ptr<BeqLabel>) override;
    void visit(std::shared_ptr<BneLabel>) override;
    void visit(std::shared_ptr<DefineLabel>) override;
    void visit(std::shared_ptr<UseLabel>) override;
    void visit(std::shared_ptr<VarAccess>) override;
    void visit(std::shared_ptr<Scope>) override;
    void visit(std::shared_ptr<IfStmt>) override;
};
