
#include "chunk.h"

#include <stdlib.h>

#include <iostream>

#include "assembly.h"
#include "block.h"
#include "reg.h"
#include "word.h"

struct Variable;

Chunk::Chunk(std::vector<std::shared_ptr<Variable>> variables) :
    variables {variables},
    words {static_cast<uint32_t>(variables.size() + 1)},
    bytes {static_cast<uint32_t>(4 * (variables.size() + 1))} {}

uint32_t Chunk::get_offset(std::shared_ptr<Variable>& variable) {
    uint32_t offset = 4;
    for (auto& v : variables) {
        if (variable == v) {
            return offset;
        }
        offset += 4;
    }
    std::cerr << "Variable not found in chunk: " << variable->name << std::endl;
    exit(1);
}

std::shared_ptr<Code>
Chunk::load(Reg base, Reg reg, std::shared_ptr<Variable>& variable) {
    return make_lw(reg, get_offset(variable), base);
}

std::shared_ptr<Code>
Chunk::load_address(Reg base, Reg reg, std::shared_ptr<Variable>& variable) {
    return make_block(
        {make_lis(Reg::Scratch3),
         make_word(get_offset(variable)),
         make_add(reg, base, Reg::Scratch3)}
    );
}

std::shared_ptr<Code>
Chunk::store(Reg base, std::shared_ptr<Variable>& variable, Reg reg) {
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
