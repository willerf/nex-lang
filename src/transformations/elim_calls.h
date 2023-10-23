
#pragma once

#include "visitor.h"

#include "procedure.h"
#include "chunk.h"
#include "call.h"

#include <map>

class ElimCalls : public Visitor<std::shared_ptr<Code>> {
    std::shared_ptr<Procedure> current_procedure;
    std::map<std::shared_ptr<Procedure>, Chunk> param_chunks;
public:
    explicit ElimCalls(std::shared_ptr<Procedure> current_procedure, std::map<std::shared_ptr<Procedure>, Chunk> param_chunks);
    std::shared_ptr<Code> visit(std::shared_ptr<Call>) override;
};

