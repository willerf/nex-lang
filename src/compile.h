
#pragma once

#include <vector>
#include <memory>
#include <string>
#include "code.h"

std::vector<std::shared_ptr<Code>> compile(std::string input);
