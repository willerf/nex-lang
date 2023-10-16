
#pragma once

#include "visitor.h"

#include "../memory_management/chunk.h"
#include "../program_representation/code.h"
#include "../program_representation/code_structures/block.h"
#include "../program_representation/code_structures/scope.h"

class ElimScopes : public Visitor<std::shared_ptr<Code>> {
    std::vector<std::shared_ptr<Variable>> variables;
public:
    std::shared_ptr<Code> visit(std::shared_ptr<Code>) override;
    std::shared_ptr<Code> visit(std::shared_ptr<Block>) override;
    std::shared_ptr<Code> visit(std::shared_ptr<Scope>) override;
    std::vector<std::shared_ptr<Variable>> get();
};

