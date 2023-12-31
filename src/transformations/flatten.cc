
#include "flatten.h"

void Flatten::visit(std::shared_ptr<Code> code) {
    result.push_back(code);
}

void Flatten::visit(std::shared_ptr<Block> block) {
    for (auto& c : block->code) {
        c->accept(*this);
    }
}

std::vector<std::shared_ptr<Code>> Flatten::get() {
    return result;
}
