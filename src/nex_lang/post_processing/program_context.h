
#pragma once

#include "module_table.h"
#include "type_table.h"

struct ProgramContext {
    ModuleTable module_table;
    TypeTable type_table;
};
