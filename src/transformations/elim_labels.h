
#pragma once

#include "../program_representation/code.h"

#include <vector>

std::vector<std::shared_ptr<Code>> elim_labels(std::vector<std::shared_ptr<Code>> program);

