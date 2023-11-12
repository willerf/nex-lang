
#pragma once

#include <vector>
#include <memory>

#include "code.h"

std::vector<std::shared_ptr<Code>>
elim_labels(std::vector<std::shared_ptr<Code>> program);
