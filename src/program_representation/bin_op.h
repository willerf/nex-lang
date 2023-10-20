
#include "assembly.h"
#include "block.h"
#include "code.h"

std::shared_ptr<Code> bin_op(
    std::shared_ptr<Code> e1,
    std::shared_ptr<Code> op,
    std::shared_ptr<Code> e2
);

namespace op {
std::shared_ptr<Code> plus();
std::shared_ptr<Code> minus();
std::shared_ptr<Code> times();
std::shared_ptr<Code> divide();
std::shared_ptr<Code> remainder();
std::shared_ptr<Code> divide_unsigned();
std::shared_ptr<Code> remainder_unsigned();
}  // namespace op
