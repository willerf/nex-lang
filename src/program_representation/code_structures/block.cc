
#include "block.h"

struct Code;

Block::Block(std::vector<std::shared_ptr<Code>> code) : code {code} {}

std::shared_ptr<Block> make_block(std::vector<std::shared_ptr<Code>> code) {
    return std::make_shared<Block>(code);
}
