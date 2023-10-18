
#pragma once

#include "chunk.h"
#include "code.h"

#include <memory>

namespace stack {
    std::shared_ptr<Code> allocate(Chunk chunk);
    std::shared_ptr<Code> pop();
}
