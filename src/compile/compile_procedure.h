
#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "chunk.h"
#include "code.h"
#include "procedure.h"

void compile_procedure(
    std::shared_ptr<Procedure> proc,
    std::map<std::shared_ptr<Procedure>, std::shared_ptr<Chunk>> param_chunks
);
