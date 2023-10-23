
#include "reg.h"
#include "assembly.h"

std::string to_string(Reg reg) {
    std::string result;

    switch (reg) {
        case Reg::Zero:
            result = "Zero";
            break;
        case Reg::Input1:
            result = "Input1";
            break;
        case Reg::Input2:
            result = "Input2";
            break;
        case Reg::Result:
            result = "Result";
            break;
        case Reg::Scratch:
            result = "Scratch";
            break;
        case Reg::SavedParamPtr:
            result = "SavedParamPtr";
            break;
        case Reg::CopyChunkScratch:
            result = "CopyChunkScratch";
            break;
        case Reg::TargetPC:
            result = "TargetPC";
            break;
        case Reg::ScratchPtrForGC:
            result = "ScratchPtrForGC";
            break;
        case Reg::FromSpaceEnd:
            result = "FromSpaceEnd";
            break;
        case Reg::HeapPtr:
            result = "HeapPtr";
            break;
        case Reg::FramePtr:
            result = "FramePtr";
            break;
        case Reg::StackPtr:
            result = "StackPtr";
            break;
        case Reg::Link:
            result = "Link";
            break;
    }

    return result;
}

std::ostream& operator<<(std::ostream& out, const Reg reg) {
    out << to_string(reg);
    return out;
}


