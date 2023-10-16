
#include "word.h"

Word::Word(uint32_t bits) : bits{bits} {}

std::shared_ptr<Code> make_word(uint32_t bits) {
    return std::make_shared<Word>(bits);
}

