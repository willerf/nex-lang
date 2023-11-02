
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "code.h"

std::vector<std::shared_ptr<Code>> compile(std::string input);
