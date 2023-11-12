#include "scanning_error.h"

#include <string>

ScanningError::ScanningError(size_t line_no) :
    CompileError(
        "Scanning error on line: " + std::to_string(line_no),
        line_no
    ) {}
