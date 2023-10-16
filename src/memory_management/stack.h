
#pragma once


#pragma once

#include "chunk.h"
#include "../program_representation/code.h"

#include <memory>

namespace stack {
    std::shared_ptr<Code> allocate(Chunk chunk);
    std::shared_ptr<Code> pop();
}
