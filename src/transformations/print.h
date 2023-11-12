
#pragma once

#include <memory>
#include <vector>

#include "beq_label.h"
#include "block.h"
#include "bne_label.h"
#include "code.h"
#include "define_label.h"
#include "if_stmt.h"
#include "scope.h"
#include "use_label.h"
#include "var_access.h"
#include "visitor.h"
#include "word.h"

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
