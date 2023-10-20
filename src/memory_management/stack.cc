
#include "stack.h"

#include "assembly.h"
#include "block.h"
#include "reg.h"
#include "word.h"

std::shared_ptr<Code> stack::allocate(Chunk chunk) {
    std::vector<std::shared_ptr<Code>> result = {
        make_lis(Reg::Scratch),
        make_word(chunk.bytes),
        make_sub(Reg::StackPtr, Reg::StackPtr, Reg::Scratch),
        make_add(Reg::Result, Reg::StackPtr, Reg::Zero),
        chunk.initialize()};
    return make_block(result);
}

std::shared_ptr<Code> stack::pop() {
    std::vector<std::shared_ptr<Code>> result = {
        make_lw(Reg::Scratch, 0, Reg::StackPtr),
        make_add(Reg::StackPtr, Reg::StackPtr, Reg::Scratch)};
    return make_block(result);
}
