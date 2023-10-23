
#pragma once

#include "reg.h"
#include <memory>
#include "code.h"
#include "variable.h"

std::shared_ptr<Code> to_expr(Reg reg);
std::shared_ptr<Code> int_literal(int32_t);
std::shared_ptr<Code> assign(std::shared_ptr<Variable> var, std::shared_ptr<Code> expr);
