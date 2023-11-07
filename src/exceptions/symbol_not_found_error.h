#pragma once

#include <stdexcept>
#include <string>

class SymbolNotFoundError: public std::runtime_error {
  public:
    SymbolNotFoundError(const std::string& symbol);
};
