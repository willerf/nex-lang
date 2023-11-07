
#pragma once

#include <stdexcept>
#include <string>

class TypeMismatchError: public std::runtime_error {
  public:
    TypeMismatchError(const std::string& message);
};
