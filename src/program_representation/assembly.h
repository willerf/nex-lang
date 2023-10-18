
#pragma once

#include "code.h"

#include "reg.h"

#include <memory>
#include <cassert>

// compiled Bit Value Sequence (BVS)
template <uint32_t Start, uint32_t End, uint32_t Value>
struct BVS {
    static_assert(Value < (1 << (Start - End)));
    static constexpr uint32_t val = Value << End;
};

// runtime Bit Value Sequence (BVS)
uint32_t bvs(uint32_t start, uint32_t end, uint32_t value);

std::shared_ptr<Code> make_add(Reg d, Reg s, Reg t); 
std::shared_ptr<Code> make_sub(Reg d, Reg s, Reg t); 
std::shared_ptr<Code> make_mult(Reg s, Reg t);
std::shared_ptr<Code> make_multu(Reg s, Reg t);
std::shared_ptr<Code> make_div(Reg s, Reg t);
std::shared_ptr<Code> make_divu(Reg s, Reg t);
std::shared_ptr<Code> make_mfhi(Reg d);
std::shared_ptr<Code> make_mflo(Reg d);
std::shared_ptr<Code> make_lis(Reg d);
std::shared_ptr<Code> make_lw(Reg t, uint32_t i, Reg s);
std::shared_ptr<Code> make_sw(Reg t, uint32_t i, Reg s);
std::shared_ptr<Code> make_slt(Reg d, Reg s, Reg t);
std::shared_ptr<Code> make_sltu(Reg d, Reg s, Reg t);
std::shared_ptr<Code> make_beq(Reg s, Reg t, uint32_t i); 
std::shared_ptr<Code> make_bne(Reg s, Reg t, uint32_t i); 
std::shared_ptr<Code> make_jr(Reg s);
std::shared_ptr<Code> make_jalr(Reg s);


