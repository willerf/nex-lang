
#pragma once

#include <memory>

#include "symbol_table.h"
#include "typed_procedure.h"

const std::string heap_allocate_id = "heap_allocate";
const std::string heap_free_id = "heap_free";
const std::string heap_module_id = "heap";

std::shared_ptr<Code> init_heap(std::shared_ptr<Code> heap_start);
std::shared_ptr<TypedProcedure>
make_heap_allocate(std::shared_ptr<Code> heap_start);
std::shared_ptr<TypedProcedure> make_heap_free(std::shared_ptr<Code> heap_start
);
