
#pragma once

#include "code.h"
#include "word.h"

std::vector<uint32_t> word_to_uint(std::vector<std::shared_ptr<Code>> program);
std::string emulate(std::string file_path, int32_t input1, int32_t input2);

