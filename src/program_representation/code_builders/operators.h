
#pragma once

#include <memory>

#include "code.h"

namespace op {
// binary operators
std::shared_ptr<Code> plus();
std::shared_ptr<Code> minus();
std::shared_ptr<Code> times();
std::shared_ptr<Code> divide();
std::shared_ptr<Code> remainder();
std::shared_ptr<Code> divide_unsigned();
std::shared_ptr<Code> remainder_unsigned();

std::shared_ptr<Code> and_bool();
std::shared_ptr<Code> or_bool();

std::shared_ptr<Code> eq_cmp();
std::shared_ptr<Code> ne_cmp();
std::shared_ptr<Code> lt_cmp();
std::shared_ptr<Code> gt_cmp();
std::shared_ptr<Code> le_cmp();
std::shared_ptr<Code> ge_cmp();

// unary operators
std::shared_ptr<Code> not_bool();
}  // namespace op
