
#include "compile_error.h"

CompileError::CompileError(const std::string& message, size_t line_no) :
    std::runtime_error(message),
    line_no {line_no} {}

size_t CompileError::get_line_no() {
    return line_no;
}
