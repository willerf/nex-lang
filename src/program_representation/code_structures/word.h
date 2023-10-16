
#pragma once

#include "../code_visit.h"

#include "stdint.h"

struct Word : CodeVisit<Word> {
    uint32_t bits;

    Word(uint32_t bits); 
};

std::shared_ptr<Code> make_word(uint32_t bits); 

