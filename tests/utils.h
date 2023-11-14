
#pragma once

#include <stdint.h>

#include <memory>
#include <string>
#include <vector>

#include "code.h"
#include "word.h"

struct Code;

std::vector<uint32_t> word_to_uint(std::vector<std::shared_ptr<Code>> program);
std::string emulate(std::string file_path, int32_t input1, int32_t input2);
std::vector<std::shared_ptr<Code>> compile_test(std::string input);
