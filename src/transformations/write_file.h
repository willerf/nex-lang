
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "code.h"

void write_file(
    std::string file_name,
    std::vector<std::shared_ptr<Code>>& program
);
