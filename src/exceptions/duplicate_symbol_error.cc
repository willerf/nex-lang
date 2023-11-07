
#include "duplicate_symbol_error.h"

DuplicateSymbolError::DuplicateSymbolError(const std::string& symbol) :
    std::runtime_error("Duplicate symbols found: " + symbol) {}
