#pragma once

#include "compile_error.h"

class ScanningError: public CompileError {
  public:
    ScanningError(size_t line_no);
};
