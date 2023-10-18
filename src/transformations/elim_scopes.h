
#pragma once

#include "visitor.h"

#include "chunk.h"
#include "code.h"
#include "block.h"
#include "scope.h"

class ElimScopes : public Visitor<std::shared_ptr<Code>> {
    std::vector<std::shared_ptr<Variable>> variables;
public:
    std::shared_ptr<Code> visit(std::shared_ptr<Code>) override;
    std::shared_ptr<Code> visit(std::shared_ptr<Block>) override;
    std::shared_ptr<Code> visit(std::shared_ptr<Scope>) override;
    std::vector<std::shared_ptr<Variable>> get();
};

