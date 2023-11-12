
#pragma once

#include <memory>

#include "chunk.h"
#include "code.h"
#include "procedure.h"
#include "variable.h"

std::shared_ptr<Code> add_entry_exit(
    std::shared_ptr<Procedure> procedure,
    std::shared_ptr<Chunk> frame
);
