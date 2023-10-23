
#include "call.h"

Call::Call(std::shared_ptr<Procedure> procedure, std::vector<std::shared_ptr<Code>> arguments) : procedure{procedure}, arguments{arguments} {}

std::shared_ptr<Code> make_call(std::shared_ptr<Procedure> procedure, std::vector<std::shared_ptr<Code>> arguments) {
    return std::make_shared<Call>(procedure, arguments);
}
