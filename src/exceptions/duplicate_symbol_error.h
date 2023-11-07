#pragma once

#include <stdexcept>
#include <string>

class DuplicateSymbolError: public std::runtime_error {
  public:
    DuplicateSymbolError(const std::string& symbol);
};
