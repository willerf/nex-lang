
#include "type_mismatch_error.h"

TypeMismatchError::TypeMismatchError(
    const std::string& message,
    size_t line_no
) :
    CompileError("Type Mismatch Error: " + message, line_no) {}
