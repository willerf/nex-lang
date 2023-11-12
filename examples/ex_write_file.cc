
#include <stdint.h>

#include <memory>
#include <string>
#include <vector>

#include "assembly.h"
#include "reg.h"
#include "word.h"
#include "write_file.h"

struct Code;

const uint32_t TERMINATION_PC = 0b11111110111000011101111010101101;

int main() {
    std::vector<std::shared_ptr<Code>> program = {
        make_add(Reg::Result, Reg::Input1, Reg::Input2),
        make_lis(Reg::TargetPC),
        make_word(TERMINATION_PC),
        make_jr(Reg::TargetPC)
    };

    write_file("test_write_file.bin", program);

    return 0;
}
