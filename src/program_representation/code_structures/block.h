
#pragma once

#include "code_visit.h"

#include <vector>
#include <memory>

struct Block : CodeVisit<Block> {
    std::vector<std::shared_ptr<Code>> code;

    explicit Block(std::vector<std::shared_ptr<Code>> code = {});
};

std::shared_ptr<Block> make_block(std::vector<std::shared_ptr<Code>> code = {});

