
#ifndef REG_H
#define REG_H

#include <string>
#include <iostream>

enum class Reg {
    Zero = 0,
    Input1 = 1,
    Input2 = 2,
    Result = 3,
    Scratch = 4,
    SavedParamPtr = 5,
    CopyChunkScratch = 7,
    TargetPC = 8,
    ScratchPtrForGC = 9,
    FromSpaceEnd = 27,
    HeapPtr = 28,
    FramePtr = 29,
    StackPtr = 30,
    Link = 31
};

std::string to_string(Reg);
std::ostream& operator<<(std::ostream&, const Reg);

#endif