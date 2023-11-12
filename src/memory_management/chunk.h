
#pragma once

#include <stdint.h>

#include <memory>
#include <vector>

#include "code.h"
#include "reg.h"
#include "variable.h"

enum class Reg;
struct Variable;

class Chunk {
    uint32_t get_offset(std::shared_ptr<Variable>& variable);

  public:
    std::vector<std::shared_ptr<Variable>> variables;
    const uint32_t words;
    const uint32_t bytes;
    explicit Chunk(std::vector<std::shared_ptr<Variable>> variables);

    std::shared_ptr<Code>
    load(Reg base, Reg reg, std::shared_ptr<Variable>& variable);
    std::shared_ptr<Code>
    load_address(Reg base, Reg reg, std::shared_ptr<Variable>& variable);
    std::shared_ptr<Code>
    store(Reg base, std::shared_ptr<Variable>& variable, Reg reg);

    std::shared_ptr<Code> initialize();
};
