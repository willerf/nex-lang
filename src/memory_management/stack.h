
#pragma once

#include <memory>

#include "chunk.h"
#include "code.h"

namespace stack {
std::shared_ptr<Code> allocate(std::shared_ptr<Chunk> chunk);
std::shared_ptr<Code> pop();
}  // namespace stack
