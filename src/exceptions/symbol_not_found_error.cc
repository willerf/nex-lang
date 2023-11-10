#include "symbol_not_found_error.h"

SymbolNotFoundError::SymbolNotFoundError(
    const std::string& symbol,
    size_t line_no
) :
    CompileError("Symbol does not exist: " + symbol, line_no) {}
