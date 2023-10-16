
#include "chunk.h"
#include "../program_representation/assembly.h"
#include "../program_representation/code_structures/word.h"
#include "../program_representation/code_structures/block.h"

#include <iostream>
#include <algorithm>

Chunk::Chunk(std::vector<std::shared_ptr<Variable>> variables) : variables{variables}, words{static_cast<uint32_t>(variables.size() + 2)}, bytes{static_cast<uint32_t>(4 * (variables.size() + 2))} {
    std::sort(this->variables.begin(), this->variables.end(), 
        [](const std::shared_ptr<Variable>& v1, const std::shared_ptr<Variable>& v2){
            return v1->is_pointer;
        }
    );
}


uint32_t Chunk::get_offset(std::shared_ptr<Variable>& variable) {
    uint32_t offset = 8;
    for (auto& v : variables) {
        if (variable == v) {
            return offset;
        }
        offset += 4;
    }
    std::cerr << "Variable not found in chunk!" << std::endl;
    return -1;
}

std::shared_ptr<Code> Chunk::load(Reg base, Reg reg, std::shared_ptr<Variable>& variable) {
    return make_lw(reg, get_offset(variable), base);
}

std::shared_ptr<Code> Chunk::store(Reg base, std::shared_ptr<Variable>& variable, Reg reg) {
    return make_sw(reg, get_offset(variable), base);
}

std::shared_ptr<Code> Chunk::initialize() {
    std::vector<std::shared_ptr<Code>> result;
    result.push_back(make_lis(Reg::Scratch));
    result.push_back(make_word(bytes));
    result.push_back(make_sw(Reg::Scratch, 0, Reg::Result));

    for (auto& v : variables) {
        result.push_back(store(Reg::Result, v, Reg::Zero));
    }

    return make_block(result);
}

