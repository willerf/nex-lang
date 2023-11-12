
#pragma once

#include <stdint.h>

#include <memory>

#include "code.h"
#include "reg.h"
#include "variable.h"

enum class Reg;

std::shared_ptr<Code> to_expr(Reg reg);
std::shared_ptr<Code> int_literal(int32_t);
std::shared_ptr<Code>
assign(std::shared_ptr<Variable> var, std::shared_ptr<Code> expr);
std::shared_ptr<Code> assign_to_address(
    std::shared_ptr<Code> addr,
    std::shared_ptr<Code> expr,
    uint32_t offset = 0
);
std::shared_ptr<Code> deref(std::shared_ptr<Code> expr, uint32_t offset = 0);
