
#include "type_mismatch_error.h"

TypeMismatchError::TypeMismatchError(const std::string& message) :
    std::runtime_error("Type Mismatch Error: " + message) {}
