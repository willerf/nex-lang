
#include "assembly.h"

#include <cassert>

#include "code_structures/word.h"

enum class Reg;

// runtime Bit Value Sequence (BVS)
uint32_t bvs(uint32_t start, uint32_t end, uint32_t value) {
    assert(value < (1 << (start - end)));
    return value << end;
}

std::shared_ptr<Code> make_add(Reg d, Reg s, Reg t) {
    return std::make_shared<Word>(
        BVS<32, 26, 0b000000>::val | bvs(26, 21, (uint32_t)s)
        | bvs(21, 16, (uint32_t)t) | bvs(16, 11, (uint32_t)d)
        | BVS<11, 0, 0b00000100000>::val
    );
}

std::shared_ptr<Code> make_sub(Reg d, Reg s, Reg t) {
    return std::make_shared<Word>(
        BVS<32, 26, 0b000000>::val | bvs(26, 21, (uint32_t)s)
        | bvs(21, 16, (uint32_t)t) | bvs(16, 11, (uint32_t)d)
        | BVS<11, 0, 0b00000100010>::val
    );
}

std::shared_ptr<Code> make_mult(Reg s, Reg t) {
    return std::make_shared<Word>(
        BVS<32, 26, 0b000000>::val | bvs(26, 21, (uint32_t)s)
        | bvs(21, 16, (uint32_t)t) | BVS<16, 0, 0b0000000000011000>::val
    );
}

std::shared_ptr<Code> make_multu(Reg s, Reg t) {
    return std::make_shared<Word>(
        BVS<32, 26, 0b000000>::val | bvs(26, 21, (uint32_t)s)
        | bvs(21, 16, (uint32_t)t) | BVS<16, 0, 0b0000000000011001>::val
    );
}

std::shared_ptr<Code> make_div(Reg s, Reg t) {
    return std::make_shared<Word>(
        BVS<32, 26, 0b000000>::val | bvs(26, 21, (uint32_t)s)
        | bvs(21, 16, (uint32_t)t) | BVS<16, 0, 0b0000000000011010>::val
    );
}

std::shared_ptr<Code> make_divu(Reg s, Reg t) {
    return std::make_shared<Word>(
        BVS<32, 26, 0b000000>::val | bvs(26, 21, (uint32_t)s)
        | bvs(21, 16, (uint32_t)t) | BVS<16, 0, 0b0000000000011011>::val
    );
}

std::shared_ptr<Code> make_mfhi(Reg d) {
    return std::make_shared<Word>(
        BVS<32, 16, 0b0000000000000000>::val | bvs(16, 11, (uint32_t)d)
        | BVS<11, 0, 0b00000010000>::val
    );
}

std::shared_ptr<Code> make_mflo(Reg d) {
    return std::make_shared<Word>(
        BVS<32, 16, 0b0000000000000000>::val | bvs(16, 11, (uint32_t)d)
        | BVS<11, 0, 0b00000010010>::val
    );
}

std::shared_ptr<Code> make_lis(Reg d) {
    return std::make_shared<Word>(
        BVS<32, 16, 0b0000000000000000>::val | bvs(16, 11, (uint32_t)d)
        | BVS<11, 0, 0b00000010100>::val
    );
}

std::shared_ptr<Code> make_lw(Reg t, uint32_t i, Reg s) {
    return std::make_shared<Word>(
        BVS<32, 26, 0b100011>::val | bvs(26, 21, (uint32_t)s)
        | bvs(21, 16, (uint32_t)t) | bvs(16, 0, i)
    );
}

std::shared_ptr<Code> make_sw(Reg t, uint32_t i, Reg s) {
    return std::make_shared<Word>(
        BVS<32, 26, 0b101011>::val | bvs(26, 21, (uint32_t)s)
        | bvs(21, 16, (uint32_t)t) | bvs(16, 0, i)
    );
}

std::shared_ptr<Code> make_slt(Reg d, Reg s, Reg t) {
    return std::make_shared<Word>(
        BVS<32, 26, 0b000000>::val | bvs(26, 21, (uint32_t)s)
        | bvs(21, 16, (uint32_t)t) | bvs(16, 11, (uint32_t)d)
        | BVS<11, 0, 0b00000101010>::val
    );
}

std::shared_ptr<Code> make_sltu(Reg d, Reg s, Reg t) {
    return std::make_shared<Word>(
        BVS<32, 26, 0b000000>::val | bvs(26, 21, (uint32_t)s)
        | bvs(21, 16, (uint32_t)t) | bvs(16, 11, (uint32_t)d)
        | BVS<11, 0, 0b00000101011>::val
    );
}

std::shared_ptr<Code> make_beq(Reg s, Reg t, uint32_t i) {
    return std::make_shared<Word>(
        BVS<32, 26, 0b000100>::val | bvs(26, 21, (uint32_t)s)
        | bvs(21, 16, (uint32_t)t) | bvs(16, 0, i)
    );
}

std::shared_ptr<Code> make_bne(Reg s, Reg t, uint32_t i) {
    return std::make_shared<Word>(
        BVS<32, 26, 0b000101>::val | bvs(26, 21, (uint32_t)s)
        | bvs(21, 16, (uint32_t)t) | bvs(16, 0, i)
    );
}

std::shared_ptr<Code> make_jr(Reg s) {
    return std::make_shared<Word>(
        BVS<32, 26, 0b000000>::val | bvs(26, 21, (uint32_t)s)
        | BVS<21, 0, 0b000000000000000001000>::val
    );
}

std::shared_ptr<Code> make_jalr(Reg s) {
    return std::make_shared<Word>(
        BVS<32, 26, 0b000000>::val | bvs(26, 21, (uint32_t)s)
        | BVS<21, 0, 0b000000000000000001001>::val
    );
}
