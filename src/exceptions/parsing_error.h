#pragma once

#include "compile_error.h"

class ParsingError: public CompileError {
  public:
    ParsingError(size_t line_no);
};
