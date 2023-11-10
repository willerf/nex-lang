
#include "duplicate_symbol_error.h"

DuplicateSymbolError::DuplicateSymbolError(
    const std::string& symbol,
    size_t line_no
) :
    CompileError("Duplicate symbols found: " + symbol, line_no) {}
