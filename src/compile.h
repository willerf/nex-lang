
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "code.h"

std::vector<std::shared_ptr<Code>>
compile(std::vector<std::string> input_file_paths);
