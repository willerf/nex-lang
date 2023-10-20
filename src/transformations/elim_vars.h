
#pragma once

#include "chunk.h"
#include "visitor.h"

class ElimVars: public Visitor<std::shared_ptr<Code>> {
    Chunk frame;

  public:
    ElimVars(Chunk frame);
    std::shared_ptr<Code> visit(std::shared_ptr<Code>) override;
    std::shared_ptr<Code> visit(std::shared_ptr<Block>) override;
    std::shared_ptr<Code> visit(std::shared_ptr<VarAccess>) override;
};
