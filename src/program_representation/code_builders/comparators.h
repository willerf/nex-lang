
#pragma once

#include "code.h"
#include "label.h"

#include <memory>

std::shared_ptr<Code> eq_cmp(std::shared_ptr<Label> label);
std::shared_ptr<Code> ne_cmp(std::shared_ptr<Label> label);
std::shared_ptr<Code> lt_cmp(std::shared_ptr<Label> label);
std::shared_ptr<Code> gt_cmp(std::shared_ptr<Label> label);
std::shared_ptr<Code> le_cmp(std::shared_ptr<Label> label);
std::shared_ptr<Code> ge_cmp(std::shared_ptr<Label> label);
std::shared_ptr<Code> gt_unsigned_cmp(std::shared_ptr<Label> label);
