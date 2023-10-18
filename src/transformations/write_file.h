
#pragma once

#include "code.h"

#include <string>
#include <vector>
#include <memory>

void write_file(std::string file_name, std::vector<std::shared_ptr<Code>>& program);

