
#pragma once

#include <memory>
#include "typed_procedure.h"

std::shared_ptr<Code> init_heap(std::shared_ptr<Code> heap_start);
std::shared_ptr<TypedProcedure> make_heap_allocate(std::shared_ptr<Code> heap_start, std::shared_ptr<Code> heap_end);
std::shared_ptr<TypedProcedure> make_heap_free(std::shared_ptr<Code> heap_start);

