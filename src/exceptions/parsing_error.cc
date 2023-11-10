#include "parsing_error.h"

ParsingError::ParsingError(size_t line_no) :
    CompileError("Parsing error on line: " + std::to_string(line_no), line_no) {
}
