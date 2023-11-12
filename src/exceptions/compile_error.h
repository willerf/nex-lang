
#pragma once

#include <stddef.h>

#include <stdexcept>
#include <string>

class CompileError: public std::runtime_error {
    size_t line_no;

  public:
    CompileError(const std::string& message, size_t line_no);
    size_t get_line_no();
};
