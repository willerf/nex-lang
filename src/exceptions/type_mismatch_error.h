
#pragma once

#include <stddef.h>

#include <string>

#include "compile_error.h"

class TypeMismatchError: public CompileError {
  public:
    TypeMismatchError(const std::string& message, size_t line_no);
};
