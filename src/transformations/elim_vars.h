
#pragma once

#include <memory>

#include "chunk.h"
#include "visitor.h"
#include "block.h"
#include "code.h"
#include "var_access.h"

class ElimVars: public Visitor<std::shared_ptr<Code>> {
    std::shared_ptr<Chunk> frame;

  public:
    ElimVars(std::shared_ptr<Chunk> frame);
    std::shared_ptr<Code> visit(std::shared_ptr<Code>) override;
    std::shared_ptr<Code> visit(std::shared_ptr<Block>) override;
    std::shared_ptr<Code> visit(std::shared_ptr<VarAccess>) override;
};
