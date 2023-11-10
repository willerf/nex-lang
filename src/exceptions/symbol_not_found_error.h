#pragma once

#include "compile_error.h"

class SymbolNotFoundError: public CompileError {
  public:
    SymbolNotFoundError(const std::string& symbol, size_t line_no);
};
