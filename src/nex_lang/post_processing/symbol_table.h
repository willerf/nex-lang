#pragma once

#include <map>
#include <string>

#include "typed_procedure.h"
#include "typed_variable.h"

using SymbolTable = std::map<std::string, std::shared_ptr<TypedID>>;

std::vector<std::shared_ptr<TypedID>>
symbol_table_sub(SymbolTable& symbol_table_one, SymbolTable& symbol_table_two);
