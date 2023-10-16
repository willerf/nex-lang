
#pragma once

#include "visitor.h"

#include <vector>

class Flatten : public Visitor<void> {
    std::vector<std::shared_ptr<Code>> result;
public:
    void visit(std::shared_ptr<Code>) override;
    void visit(std::shared_ptr<Block>) override;

    std::vector<std::shared_ptr<Code>> get();
};

