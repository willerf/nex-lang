
#pragma once

#include <memory>

#include "block.h"
#include "chunk.h"
#include "code.h"
#include "var_access.h"
#include "variable.h"
#include "visitor.h"

class ElimVarsProc: public Visitor<std::shared_ptr<Code>> {
    std::shared_ptr<Chunk> frame;
    std::shared_ptr<Chunk> param_chunk;
    std::shared_ptr<Variable> param_ptr;

  public:
    ElimVarsProc(
        std::shared_ptr<Chunk> frame,
        std::shared_ptr<Chunk> param_chunk,
        std::shared_ptr<Variable> param_ptr
    );
    std::shared_ptr<Code> visit(std::shared_ptr<Code>) override;
    std::shared_ptr<Code> visit(std::shared_ptr<Block>) override;
    std::shared_ptr<Code> visit(std::shared_ptr<VarAccess>) override;
};
