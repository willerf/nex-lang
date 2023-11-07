#include "symbol_not_found_error.h"

SymbolNotFoundError::SymbolNotFoundError(const std::string& symbol) :
    std::runtime_error("Symbol does not exist: " + symbol) {}
