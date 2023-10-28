
#pragma once

#include <map>

#include "call.h"
#include "chunk.h"
#include "procedure.h"
#include "visitor.h"

class ElimCalls: public Visitor<std::shared_ptr<Code>> {
    std::shared_ptr<Procedure> current_procedure;
    std::map<std::shared_ptr<Procedure>, std::shared_ptr<Chunk>> param_chunks;

  public:
    explicit ElimCalls(
        std::shared_ptr<Procedure> current_procedure,
        std::map<std::shared_ptr<Procedure>, std::shared_ptr<Chunk>>
            param_chunks
    );
    std::shared_ptr<Code> visit(std::shared_ptr<Code>) override;
    std::shared_ptr<Code> visit(std::shared_ptr<Block>) override;
    std::shared_ptr<Code> visit(std::shared_ptr<Call>) override;
};
