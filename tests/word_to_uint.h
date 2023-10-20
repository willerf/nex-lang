
#pragma once

#include "code.h"
#include "word.h"

std::vector<uint32_t> word_to_uint(std::vector<std::shared_ptr<Code>> program) {
    std::vector<uint32_t> result;
    for (auto code : program) {
        if (auto word = std::dynamic_pointer_cast<Word>(code)) {
            result.push_back(word->bits);
        }
    }
    return result;
}
