#pragma once

#include "compile_error.h"

class DuplicateSymbolError: public CompileError {
  public:
    DuplicateSymbolError(const std::string& symbol, size_t line_no);
};
