
#pragma once

#include <memory>
#include <vector>

#include "code_visit.h"

struct Code;

struct Block: CodeVisit<Block> {
    std::vector<std::shared_ptr<Code>> code;

    explicit Block(std::vector<std::shared_ptr<Code>> code = {});
};

std::shared_ptr<Block> make_block(std::vector<std::shared_ptr<Code>> code = {});
