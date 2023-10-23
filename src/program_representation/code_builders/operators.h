
#pragma once

#include "code.h"

namespace op {
std::shared_ptr<Code> plus();
std::shared_ptr<Code> minus();
std::shared_ptr<Code> times();
std::shared_ptr<Code> divide();
std::shared_ptr<Code> remainder();
std::shared_ptr<Code> divide_unsigned();
std::shared_ptr<Code> remainder_unsigned();
}  // namespace op
